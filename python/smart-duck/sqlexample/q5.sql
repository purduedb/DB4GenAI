SELECT
    llm(
        format(
            'Given the following context {}, answer this question {}',
            semantic_search(s.question),
            s.question,
            false
        )
    )
FROM
    squad s
WHERE
    s.is_impossible = false;