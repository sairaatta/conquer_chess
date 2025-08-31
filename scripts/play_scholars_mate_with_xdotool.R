#!/bin/env Rscript
if (1 == 2) {
  remotes::install_github("pmur002/xdotool")
}

message("You have three seconds to put the Conquer Chess window in focus")

sleep_time_secs <- 8.0
message("This script assumes that ", sleep_time_secs, " second(s) == 1 chess move")
Sys.sleep(3.0) # seconds

#
# "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? 4. Qxf7# 1-0"
#     --
#
# White cursor at e1, black at e8
# White is to move
xdotool::keystroke("d") # e2
xdotool::keystroke("q") # select
xdotool::keystroke("d") # e3
xdotool::keystroke("d") # e4
xdotool::keystroke("q") # go
Sys.sleep(sleep_time_secs)

# "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? 4. Qxf7# 1-0"
#        --
#
# White cursor at e4, black at e8
# Black is to move
xdotool::keystroke("j") # e7
xdotool::keystroke("u") # select
xdotool::keystroke("j") # e3
xdotool::keystroke("j") # e4
xdotool::keystroke("u") # go
Sys.sleep(sleep_time_secs)

# "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? 4. Qxf7# 1-0"
#              --
#
# White cursor at e4, black at e5
# White is to move
xdotool::keystroke("a") # e3
xdotool::keystroke("a") # e2
xdotool::keystroke("a") # e1
xdotool::keystroke("w") # d1
xdotool::keystroke("q") # select
xdotool::keystroke("d") # d2
xdotool::keystroke("d") # d3
xdotool::keystroke("d") # d4
xdotool::keystroke("d") # d5
xdotool::keystroke("s") # e5
xdotool::keystroke("s") # f5
xdotool::keystroke("s") # g5
xdotool::keystroke("s") # h5
xdotool::keystroke("q") # go
Sys.sleep(sleep_time_secs)

# "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? 4. Qxf7# 1-0"
#                  --
#
# White cursor at h5, black at e5
# Black is to move
xdotool::keystroke("l") # e6
xdotool::keystroke("l") # e7
xdotool::keystroke("l") # e8
xdotool::keystroke("i") # d8
xdotool::keystroke("i") # c8
xdotool::keystroke("i") # b8
xdotool::keystroke("u") # select

xdotool::keystroke("j") # b7
xdotool::keystroke("j") # b6
xdotool::keystroke("k") # c6
xdotool::keystroke("u") # go
Sys.sleep(sleep_time_secs)




















# "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? 4. Qxf7# 1-0"
#                         ---
#
# White cursor at h5, black at c6
# White is to move
xdotool::keystroke("a") # h4
xdotool::keystroke("a") # h3
xdotool::keystroke("a") # h2
xdotool::keystroke("a") # h1
xdotool::keystroke("w") # g1
xdotool::keystroke("w") # f1
xdotool::keystroke("q") # select
xdotool::keystroke("d") # f2
xdotool::keystroke("d") # f3
xdotool::keystroke("d") # f4
xdotool::keystroke("w") # e4
xdotool::keystroke("w") # d4
xdotool::keystroke("w") # c4
xdotool::keystroke("q") # go
Sys.sleep(sleep_time_secs)

# "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? 4. Qxf7# 1-0"
#                             --
#
# White cursor at c4, black at c6
# Black is to move
xdotool::keystroke("l") # c7
xdotool::keystroke("l") # c8
xdotool::keystroke("k") # d8
xdotool::keystroke("k") # e8
xdotool::keystroke("k") # f8
xdotool::keystroke("k") # g8
xdotool::keystroke("u") # select
xdotool::keystroke("j") # g7
xdotool::keystroke("j") # g6
xdotool::keystroke("i") # f6
xdotool::keystroke("u") # go
Sys.sleep(sleep_time_secs)




# "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? 4. Qxf7# 1-0"
#                         ---
#
# White cursor at c4, black at f6
# White is to move
xdotool::keystroke("d") # c5
xdotool::keystroke("s") # d5
xdotool::keystroke("s") # e5
xdotool::keystroke("s") # f5
xdotool::keystroke("s") # g5
xdotool::keystroke("s") # h5
xdotool::keystroke("q") # select

xdotool::keystroke("d") # h6
xdotool::keystroke("d") # h7
xdotool::keystroke("w") # f7
xdotool::keystroke("w") # e7
xdotool::keystroke("q") # go

message("Checkmate!")
