# Gripe
You may have come across questions that give you a sequence of numbers, and
then ask you to find the next number. Here is an example.

> Find the next number in this sequence. `1, 2, 4, 8, 16, …`

At a glance, it looks like the next number must be 32. However, the correct
answer entirely depends on what was going on in the mind of the person who came
up with this question (and they sometimes insist their answer is the *only*
correct answer, unless you prove the correctness of your answer). If that
person:
- was thinking about [dividing circles](https://oeis.org/A000127), the next
  number may be 31.
- likes [geometric series](https://oeis.org/A000079), 32 is a probable answer.
- studied [electrical engineering](https://oeis.org/A180414), it could be 36.
- sees [partitions](https://oeis.org/A160786) in their dreams, 29 cannot be
  ruled out.
- is a member of the interview panel of the Indian Administrative
  Services, they might have 473.34876 in mind. (Who knows!)

I have heard lots of stories about IAS interview panelists asking such trick
questions (though I don't know if they are true).
[This XKCD comic](https://xkcd.com/169) demonstrates a possible response to
trick questions. I abhor violence, so my response to sequence-related trick
questions is this program.

# Purpose
It fits a polynomial through a given sequence (and also predicts the next
term). For instance, given the numbers:
```
42, 43, 44, 45, 46, 98756
```
it should output the polynomial coefficients (constant term first):
```
-98668, 225386.55, -185079.375, 69918.875, -12338.625, 822.575
```
whereby you can claim, with proof, that the next term in the sequence
```
42, 43, 44, 45, 46, …
```
is 98756.

# Input Format Example
```
1 42
2 43
3 44
4 45
5 46
6 98756
7
```

# Usage
Enter the coordinates of the points in the file `points.txt`. The input format
is as explained in the previous section.
- Each line (except the last one) must contain two numbers: the first is the
  x-coordinate (or simply the line number if you don't want to bother with
  coordinates), and the second is the y-coordinate (i.e. the number appearing
  in the sequence).
- On the last line, write the x-coordinate at which you want to find a term of
  the sequence. For instance, in the example above, we are looking for the
  seventh term of the sequence.

Compile and run with the following commands.
```
make
./sequence points.txt
```
If you enter `./sequence points.txt 1` instead of `./sequence points.txt`, the
coefficients will be displayed in rational form.
