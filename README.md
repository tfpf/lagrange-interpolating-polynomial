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

This program aims to fit a polynomial through a given sequence to produce
hilarious results. For instance, given the sequence:
<br>
1, 2, 3, 4, 98756
<br>
the program should output the polynomial coefficients:
<br>
98751, −205730.25, 144011.875, −41146.25, 4114.625
<br>
which allows you to claim, with proof (IAS interviewers won't like this), that
the next term in the sequence
<br>
1, 2, 3, 4
<br>
is 98756.

# input format example
1 1
<br>
2 2
<br>
3 3
<br>
4 4
<br>
5 98756
<br>
6

# usage

- Install `g++` to compile this program.
- Download the files to your computer.
- Open a terminal window where you downloaded the files.
- Enter the coordinates of the points in the file 'input.txt' (the input format
  is as explained above).
  - Each line (except the last one) must contain two numbers: the second number
    is the number appearing in the sequence; the first number is the position
    the second numbers appears at.
  - The last line contains the position at which you want to find a term of the
    sequence.
- In the terminal, enter the command `make run`.

