SELECT title
FROM movies
JOIN ratings ON movies.id = ratings.movie_id
JOIN stars ON movies.id = stars.movie_id
JOIN people on stars.person_id = people.id
WHERE people.name = "Chadwick Boseman"
ORDER BY ratings.rating DESC
LIMIT 5;