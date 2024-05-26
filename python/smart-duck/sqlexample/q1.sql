SELECT
    llm(
        format(
            'Recommend movies for the user based on {} and {}',
            m.movie_info,
            r.review_content,
            true
        )
    )
FROM
    reviews r
    JOIN movies m ON (r.rotten_tomatoes_link = m.rotten_tomatoes_link);