# Groonga near function

## Install

Install libgroonga-dev.

Build this function.

    % sh autogen.sh
    % ./configure
    % make
    % sudo make install

## Usage

Register `function/near`:

    % groonga DB
    > register function/near

Now, you can use `near` function

```
select Diaries   --output_columns body,_score \
  --match_columns 'body'   --query 'tokyo' \
  --filter 'near(body, 2, "rainy tokyo")'
[
  [
    0,
    0.0,
    0.0
  ],
  [
    [
      [
        1
      ],
      [
        [
          "body",
          "ShortText"
        ],
        [
          "_score",
          "Int32"
        ]
      ],
      [
        "It was rainy in Tokyo. But it was sunny in Osaka.",
        2
      ]
    ]
  ]
]
```

## Author

Naoya Murakami naoya@createfield.com

## License

LGPL 2.1. See COPYING-LGPL-2.1 for details.
