# introduction
Often, you may come across questions that give you a sequence of numbers, and then ask you to find the next number. Here is an example.

### Find the next number in this sequence. 1, 2, 4, 8, 16, ...
At a glance, it looks like the next number must be 32. However, the correct answer entirely depends on what was going on in the mind of the person who came up with this question.
- If the person was thinking about dividing a circle into separate areas, the next number is 31.
- If the person was thinking about powers of 2, they'd say that the next number is 32.
- If the person had multiple-personality disorder, the next number could be 473.34876 (who knows).

You get the idea. Any logician will tell you that any number can be next in the sequence.

# purpose

This program aims to fit a polynomial through a given sequence to produce hilarious results. For instance, given the sequence:
<br>
1, 2, 3, 4, 98756
<br>
the program should output the polynomial coefficients:
<br>
98751, −205730.25, 144011.875, −41146.25, 4114.625
<br>
which allows you to claim, with proof (IAS interviewers won't like this), that the next term in the sequence
<br>
1, 2, 3, 4
<br>
is 98756.

