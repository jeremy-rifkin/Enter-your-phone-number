# PI

So I think we've all seen this meme:

![][meme1]

Well one day I was browsing other similar memes and I came across this one:

[![][piMeme]][piMemeLink]

Which got me thinking, how many digits of pi would be needed to contain all possible phone numbers?
For simplicity I will not worry about assigned / unassigned area codes and just assume they are all
valid. Well listing every 10 digit phone number in order would take 100 billion digits, but as John
D. Cook points out in his article [*Cracking pass codes with De Bruijn sequences*][cookArticle], the
shortest string containing all possible combinations is actually much shorter. All 10 digit phone
numbers can in fact be listed in 10 billion 9 digits (which makes sense). No doubt though, pi will
not just happen to be a De Bruijn sequence for phone numbers, so I will have to look through many
more digits.

So roughly how many digits will be needed in order to get all phone numbers? Assuming
the digits of pi are normal (which I think is a safe assumption given that a fair amount of work has
been done to show that the [digits of pi are normal][piNormal]), the probability of any 10 digits
being a given phone number is `1e-10`. The probability of a phone number not occurring in `N`
digits of pi is roughly `(1 - 1e-10) ** N`. And the probability of not every phone number occurring
in `N` digits of pi is `P(N) = (1 - (1 - 1e-10) ** N) ** 1e10`. Interestingly we can find the
expected value of `P(N)` by [integrating `1 - P(N)`][expectedValue].

```python
>>> import mpmath
>>> mpmath.mp.dps = 50
>>> q = lambda n: 1 - (1 - (1 - 1/10E9) ** (n)) ** (10E9)
>>> mpmath.quad(q, [0, mpmath.inf])
mpf('235790561308.89049113279209421732468319578834860609999')
```

So we can expect to find all 10 digit phone numbers in about `235.79` billion digits of pi.

I wrote a script to download `300` billion digits of pi (which I figured would be more than plenty
as `P(300E9)`, the probability that all phone numbers appear in the first 300 billion digits of pi
is 99.9%) from [this site][piSource], and I wrote a
program to run through those digits and see how many digits were required to find all 10 billion
10 digit phone numbers. The program uses what I think is a clever technique to get all groups of 10
digits from pi and a bitmap to keep track of which phone numbers have been found. The program, after
a running for 2 hours (I decided against multithreading as the bottleneck was mostly in hard drive),
reports that all 10 digit phone numbers appear in the first `241.6` billion digits of pi.

I was quite delighted that the actual value was so close to the expected value.

[meme1]: https://www.dailydot.com/wp-content/uploads/b53/5f/3ef75270aa2ddf2db6c45fe3a9847483.jpg
[piMeme]: https://i.imgur.com/FBXzAGH.gif
[piMemeLink]: https://imgur.com/gallery/r102w
[cookArticle]: https://www.johndcook.com/blog/2019/10/22/hacking-with-de-bruijn/
[piNormal]: https://arxiv.org/pdf/1608.00430.pdf
[expectedValue]: https://stats.stackexchange.com/a/13377
[piSource]: https://pi2e.ch/blog/2017/03/10/pi-digits-download/#download
