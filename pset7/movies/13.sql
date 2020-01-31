SELECT DISTINCT name
FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies on stars.movie_id = movies.id
-- IN operator finds any matches between query above and below
WHERE movies.id IN
-- Table of all the movies Kevin Bacon is in (nested query)
(SELECT movies.id FROM movies
JOIN stars on movies.id = stars.movie_id
JOIN people on stars.person_id = people.id
WHERE people.name = "Kevin Bacon" AND people.birth = 1958)
-- Exclude Kevin Bacon from query results
AND people.name != "Kevin Bacon";

