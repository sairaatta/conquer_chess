# Conquer Chess: Battle for Liberty

<!-- markdownlint-disable MD013 --><!-- Badges cannot be split up over lines, hence will break 80 characters per line -->

Branch     |[![GitHub Actions logo](images/GitHubActions.png)](https://github.com/richelbilderbeek/conquer_chess/actions) |[![Codecov logo](images/Codecov.png)](https://www.codecov.io)
-----------|--------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
`main`     |![check](https://github.com/richelbilderbeek/conquer_chess/workflows/check/badge.svg?branch=main)             |[![codecov.io](https://codecov.io/github/richelbilderbeek/conquer_chess/coverage.svg?branch=main)](https://codecov.io/github/richelbilderbeek/conquer_chess/branch/main)
`develop`  |![check](https://github.com/richelbilderbeek/conquer_chess/workflows/check/badge.svg?branch=develop)          |[![codecov.io](https://codecov.io/github/richelbilderbeek/conquer_chess/coverage.svg?branch=develop)](https://codecov.io/github/richelbilderbeek/conquer_chess/branch/develop)

<!-- markdownlint-enable MD013 -->

Chess variant.

- [Go to the 'Conquer Chess' site](https://richelbilderbeek.github.io/conquer_chess/)

## Files used by continuous integration scripts

<!-- markdownlint-disable MD013 --><!-- Tables cannot be split up over lines, hence will break 80 characters per line -->

Filename                                  |Description
------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------
[mlc_config.json](mlc_config.json)        |Configuration of the link checker, use `markdown-link-check --config mlc_config.json --quiet docs/**/*.md` to do link checking locally
[.spellcheck.yml](.spellcheck.yml)        |Configuration of the spell checker, use `pyspelling -c .spellcheck.yml` to do spellcheck locally
[.wordlist.txt](.wordlist.txt)            |Whitelisted words for the spell checker, use `pyspelling -c .spellcheck.yml` to do spellcheck locally
[.markdownlint.jsonc](.markdownlint.jsonc)|Configuration of the markdown linter, use `markdownlint "**/*.md"` to do markdown linting locally. The name of this file is a default name.
[.markdownlintignore](.markdownlintignore)|Files ignored by the markdown linter, use `markdownlint "**/*.md"` to do markdown linting locally. The name of this file is a default name.

<!-- markdownlint-enable MD013 -->
