SELECT DISTINCT name
FROM people
JOIN directors ON people.id = directors.person_id
JOIN ratings ON directors.movie_id = ratings.movie_id
WHERE ratings.rating >= 9.0;