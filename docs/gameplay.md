# Gameplay

This page describes how to play Conquer Chess.

## Main menu

![Main menu screen v0.8](screenshots/20250619_1.jpg)

The main menu is where you start. Here you can go to the game lobby,
change the settings of the game or view information about the game.

We need to go through the lobby to start the game.

To go to the lobby:

- Press 'S'
- Navigate to the Start button with the arrow keys and press Enter
- Click on the Start button

## Lobby

![Lobby screen v0.8](screenshots/20250619_2.jpg)

The lobby is where the match is set up.
Here the two players pick who plays which side
and which races they use.

The effect of each side is simple: white, like in regular chess, can start,
where black has to wait one chess move.

Side |Waiting time before first move
-----|-------------------------------
White|None
Black|One chess move

There are multiple differences between the races.
Here is a general overview of the races:

Logo                                        |Race    |Description
--------------------------------------------|--------|--------------------
![Classic logo](classic_logo_128_x_128.jpg) |Classic |Regular chess pieces
![Kingdom logo](kingdom_logo_128_x_128.jpg) |Kingdom |Human-like race
![Rooxx logo](rooxx_logo_128_x_128.jpg)     |Rooxx   |Advanced alien race
![Spawn logo](spawn_logo_128_x_128.jpg)     |Spawn   |Primitive alien race

The first difference between the races is the ability to withstand
damage:

<!-- markdownlint-disable MD013 --><!-- Tables cannot be split up over lines, hence will break 80 characters per line -->

Race    |Maximum health [1] |Maximum shield [2]|Other features
--------|-------------------|------------------|------------------
Classic |1.0                |0.0               |None
Kingdom |0.75               |0.0               |None
Rooxx   |0.5                |0.5               |Regenerate 0.25 shield per chess move `[4]`
Spawn   |0.5                |0.0               |Regenerates 0.25 health per chess move `[3]`

<!-- markdownlint-enable MD013 -->

- `[1]` From [the rulebook](rulebook/README.md) rule `[1.2.CC.4]`
- `[2]` From [the rulebook](rulebook/README.md) rule `[1.2.CC.5]`
- `[3]` From [the rulebook](rulebook/README.md) rule `[1.2.CC.6]`
- `[4]` From [the rulebook](rulebook/README.md) rule `[1.2.CC.7]`


The second difference between the races is the ability to move and attack:

Race    |Movement speed [1] |Attack speed [2]
--------|-------------------|------------------
Classic |1                  |1.0
Kingdom |1.0                |1.0
Rooxx   |0.9                |1.1
Spawn   |1.1                |0.9

- `[1]` From [the rulebook](rulebook/README.md) rule `[1.2.CC.8]`
- `[2]` From [the rulebook](rulebook/README.md) rule `[1.2.CC.9]`

The third difference between the races is the effect of a piece being
protected by another piece:

  Race    |Effect of being protected `[1]`
  --------|-------------------------------
  Classic |None
  Rooxx   |Increase movement speed by 20%
  Kingdom |Heal at a rate of 0.25 health per time unit
  Spawn   |Increase attack rate by 20%

- `[1]` From [the rulebook](rulebook/README.md) rule `[1.2.CC.10]`

When both players are ready, there is a countdown and the game starts.

## Game

![Game screen v0.8](screenshots/20250622_3.jpg)

The goal of the game is to:

- put the king of the opponent into a checkmate (as per regular chess)
- kill the king of the opponent, by attacking it until its health reaches zero

The game is timed in chess moves.
This time is shown directly below the board.

The pieces move exactly the same as in regular chess.

Moving a piece takes one chess move (as per regular chess):
it takes just as long to move a pawn one square forward,
as to move a queen accross the board.

If two pieces attempt to move to the same square (due to realtime speed),
the earlier piece will arrive at that square,
where the later piece will go back to its original square.

Capturing a piece takes one chess move (as per regular chess):
when a piece starts attacking, it will take one time unit
to lower the health of the attacked piece to zero.
When the health of the attacked piece reaches zero,
the attacker instantaneously moves to the square of the
captured piece.

If multiple pieces attack the same square (due to realtime speed),
the attacked piece will lose health faster.
:construction: It is the first attacker that will move to the square.

Like regular chess, a move, once started, cannot be stopped or undone.
Think before you do a move!

Castling works the same as in regular chess: one cannot castle
when in check, going through check, when the king has moved
or when the partnering rook has moved.

Detailed rules can be found in [the rulebook](rulebook/README.md).
