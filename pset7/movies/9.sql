-- DISTINCT removes duplicates
SELECT DISTINCT people.name
FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies on stars.movie_id = movies.id
WHERE movies.year = 2004
ORDER BY people.birth;