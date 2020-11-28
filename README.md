# introduction
Often, you may come across questions that give you a sequence of numbers, and
then ask you to find the next number. Here is an example.

### Find the next number in this sequence. 1, 2, 4, 8, 16, ...
At a glance, it looks like the next number must be 32. However, the correct
answer entirely depends on what was going on in the mind of the person who came
up with this question.
- If the person was thinking about dividing a circle into separate areas, the
  next number may be 31.
- If the person likes geometric series, 32 is a probable answer.
- If the person had multiple-personality disorder, they might have the number
  473.34876 in mind (who knows).

You get the idea. Any logician will tell you that any number can be next in the
sequence.

# purpose
This program fits a polynomial through a given sequence and predicts the next
term in the sequence. For instance, given the sequence:
<br>
42, 43, 44, 45, 46, 98756
<br>
the program should output the polynomial coefficients:
<br>
-98668, 225386.55, -185079.375, 69918.875, -12338.625, 822.575
<br>
which allows you to claim that the next term in the sequence
<br>
42, 43, 44, 45, 46
<br>
is 98756. I wonder if this will nonplus the interviewers of the Indian
Administrative Services examination!

# input format example
1 42
<br>
2 43
<br>
3 44
<br>
4 45
<br>
5 46
<br>
6 98756
<br>
7

# usage
- Install `g++` to compile this program.
- Download or clone this repository to some location on your computer.
- Open a terminal window to that location.
- Enter the coordinates of the points in the file 'input.txt' (the input format
  is as explained above).
  - Each line (except the last one) must contain two numbers: the first is the
    line number; the second is the number appearing in the sequence.
  - On the last line, put the position you want find a term of the sequence at.
- In the terminal, enter the command `make run`.
  - If you enter `make run r=1` instead of just `make run`, the polynomial
    coefficients will be displayed in rational form.

