#!/bin/env Rscript
if (1 == 2) {
  remotes::install_github("pmur002/xdotool")
}

get_useful_keys <- function() {
  c(
    "a", "d", "s", "w", "q", "e", "z", "c",
    "i", "j", "k", "l", "u", "o", "m", "."
  )
}

get_random_key <- function() {
  sample(get_useful_keys(), 1)
}

run <- function(n_presses = 10000) {

  Sys.sleep(3.0) # seconds

  for (i in seq_len(n_presses)) {
    key <- get_random_key()
    message(i, ": pressing key: ", key)
    xdotool::keystroke(key)
  }
}

run()
