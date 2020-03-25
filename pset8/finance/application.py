import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, make_response
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Count number of bought and sold shares for each company
    rows = db.execute("SELECT * FROM transactions WHERE uid = :uid", uid = session["user_id"])
    global company_shares
    company_shares = {}

    for row in rows:
        if row["symbol"] not in company_shares:
            company_shares[row["symbol"]] = row["amount"]
        else:
            if row["type"] == 'bought':
                company_shares[row["symbol"]] = company_shares[row["symbol"]] + row["amount"]
            elif row["type"] == 'sold':
                company_shares[row["symbol"]] = company_shares[row["symbol"]] - row["amount"]

    # Count user balance (total stocks + cash)
    row = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    total = row[0]["cash"]
    for symbol, amount in company_shares.items():
        if amount > 0:
            stock = lookup(symbol)
            total += stock["price"] * amount


    return render_template("portfolio.html",
                            shares = company_shares,
                            lookup = lookup,
                            usd = usd,
                            cash = row[0]["cash"],
                            balance = total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == 'POST':
        symbol = request.form.get("symbol")
        # Ensure symbol is valid and number of shares is a positive integer
        if lookup(symbol) == None:
            return apology("invalid symbol")

        # Ensure number of shares is a positive integer
        if int(request.form.get("shares")) < 1:
            return apology("invalid number of shares")

        stock_price = lookup(symbol)["price"]
        company = lookup(symbol)["name"]

        row = db.execute("SELECT cash FROM users WHERE id=:id", id = session["user_id"])
        user_cash = row[0]["cash"]

        # Ensure user can afford purchase
        purchase_price = stock_price * int(request.form.get("shares"))
        if purchase_price > user_cash:
            return apology("can't afford")

        # Add purchase to database
        db.execute("INSERT INTO transactions (uid, company, symbol, amount, price, type) VALUES (:uid,:company,:symbol,:amount,:price,'bought')",
                    uid = session["user_id"],
                    company = company,
                    symbol = lookup(symbol)["symbol"],
                    amount = int(request.form.get("shares")),
                    price = stock_price)

        # Update users cash
        new_cash = user_cash - purchase_price
        db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                    cash = new_cash,
                    id = session["user_id"])

        flash("Bought!")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("SELECT * FROM transactions WHERE uid = :uid", uid = session["user_id"])
    return render_template("history.html", history = rows, usd = usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username = request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == 'POST':
        symbol = request.form.get("symbol")
        stock = lookup(symbol)

        if stock == None:
            return apology("invalid symbol")

        return render_template("quoted.html",
                                company = stock["name"],
                                symbol = stock["symbol"],
                                share_price = usd(stock["price"]))
    else:
        return render_template("quote.html")

@app.route("/username-availability", methods=["POST"])
def username_available():
    """Check if username is available"""

    username = request.get_json()

    rows = db.execute("SELECT * FROM users WHERE username=:username", username = username)
    if len(rows) > 0:
        available = False
    else:
        available = True

    res = make_response(jsonify({ "available": available }))
    return res

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == 'POST':
        # Ensure username was submitted and unique (incase javascript is disabled)
        if not request.form.get("username"):
            return apology("missing username")

        rows = db.execute("SELECT * FROM users WHERE username=:username", username = request.form.get("username"))
        if len(rows) > 0:
            return apology("invalid username")

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("missing password")

        # Ensure password matches confirmation
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password's dont match")

        # Add user to database
        user_id = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                            username = request.form.get("username"),
                            hash = generate_password_hash(request.form.get("password")))

        # Remember which user has logged in
        session["user_id"] = user_id

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == 'POST':
        # Ensure a symbol is selected
        if request.form.get("symbol") == None:
            return apology("missing symbol")

        # Ensure number of shares is a positive integer
        if int(request.form.get("shares")) < 1:
            return apology("shares must be positive")

        # Ensure a valid number of shares is sold
        if int(request.form.get("shares")) > company_shares[request.form.get("symbol")]:
            return apology("too many shares")

        # Update database
        stock = lookup(request.form.get("symbol"))
        rows = db.execute("SELECT * FROM users WHERE id = :id", id = session["user_id"])
        new_cash = rows[0]["cash"] + (stock["price"] * int(request.form.get("shares")))

        db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                    cash = new_cash,
                    id = session["user_id"])
        db.execute("INSERT INTO transactions (uid, company, symbol, price, amount, type) VALUES (:uid,:company,:symbol,:price,:amount,'sold')",
                    uid = session["user_id"],
                    company = stock["name"],
                    symbol = stock["symbol"],
                    price = stock["price"],
                    amount = int(request.form.get("shares")))

        flash("Sold!")
        return redirect("/")
    else:
        return render_template("sell.html", symbols = company_shares)

@app.route("/account")
@login_required
def account():
    row = db.execute("SELECT username FROM users WHERE id = :id", id = session["user_id"])
    return render_template("account.html", user = row[0]["username"])

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Add cash"""

    if request.method == 'POST':
         # Ensure amount  is a positive integer
        if int(request.form.get("amount")) < 1:
            return apology("invalid amount")

        # Update database
        row = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        new_cash = row[0]["cash"] + int(request.form.get("amount"))
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = new_cash, id = session["user_id"])

        flash("Cash added!")
        return redirect("/")
    else:
        return render_template("cash.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
