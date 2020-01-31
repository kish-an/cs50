-- Table of Johnny Depp movies
SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Johnny Depp"
-- INTERSECT operator finds rows that match query above AND below
INTERSECT
-- Table of Helena Bonham Carter movies
SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Helena Bonham Carter"
