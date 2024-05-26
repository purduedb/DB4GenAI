SELECT
    llm(
        format(
            'Is this movie suitable for kids based on {} and {}? Answer Yes or No.',
            m.movie_info,
            r.review_content,
            true
        )
    )
FROM
    movies m
    JOIN reviews r ON r.rotten_tomatoes_link = m.rotten_tomatoes_link;