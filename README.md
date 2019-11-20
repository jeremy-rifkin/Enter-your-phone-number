# Enter your phone number

So I think we've all seen this meme:

![][meme1]

Well one day I was browsing other similar memes and I came across this one:

[![][piMeme]][piMemeLink]

Which got me thinking, how many digits of pi would be needed to contain all possible phone numbers?
In this project I write a program to answer this question. For simplicity I will just consider 10
digit phone numbers and I will not worry about assigned / unassigned area codes and just assume all
are valid.

## Estimate

So roughly how many digits should be required?

Listing every `10` digit phone number sequentially would take `100` billion digits but as John D.
Cook points out in his article [*Cracking pass codes with De Bruijn sequences*][cookArticle], the
shortest string containing all possible combinations is actually much shorter. All 10 digit phone
numbers can in fact be listed in 10 billion 9 digits (which makes sense). So the lower bound is 10
billion 9 digits.

Assuming the digits of pi are random (which I think is a safe assumption given that a fair amount of
research has been done to show the [normality of pi][piNormal]), the probability of any 10 digits
being a given phone number is `1e-10`. The probability of a phone number not occurring in `N` digits
of pi is roughly `(1 - 1e-10) ** N`. And the probability of every phone number occurring in `N`
digits of pi is `P(N) = (1 - (1 - 1e-10) ** N) ** 1e10`. Interestingly we can find the expected
value of `P(N)` by [integrating `1 - P(N)`][expectedValue].

```python
>>> import mpmath
>>> mpmath.mp.dps = 300
>>> q = lambda n: 1 - (1 - (1 - 1/10E9) ** (n)) ** (10E9)
>>> E = mpmath.quad(q, [0, mpmath.inf])
>>> str(E)[:20]
'236030646407.8551119'
```

So we can expect to find all 10 digit phone numbers in about `236.03` billion digits of pi.

## Actual value

I figured `300` billion digits of pi would be a good data set to work with as `P(300E9)`, or the
probability that all phone numbers appear in the first `300` billion digits of pi, is `99.9%`).
Instead of calculating the digits on my server, I decided it would be easier to download the digits
from [this site][piSource]. The script [`download.sh`](download.sh) automates this process.

It would be impractical to search the digits of pi for every phone number, so the solution I came up
with for figuring out how many digits are required to get every phone number is to run through the
digits and use a bitmap to track which phone numbers have been encountered.

After running for about two hours (bottleneck seems to be in hard drive speed and ram speed, but
not really worth it to parallelize this problem), the program reports that all 10 digit phone
numbers can be found in the first `241.6` billion digits of pi (`241641121048` to be exact).

I was quite delighted that the actual value was so close to the expected value.

## Conclusion

Given that every ascii character requires at least 1 byte of storage space, an interface that
requires phone numbers to be entered by finding its location in pi would be impractical as it would
require at least `225` GiB of memory + whatever overhead is associated with the interface system.
That is also the only reason why this interface is bad.

## License

Copyright (c) Jeremy Rifkin 2019, licensed under the MIT license.

[meme1]: https://www.dailydot.com/wp-content/uploads/b53/5f/3ef75270aa2ddf2db6c45fe3a9847483.jpg
[piMeme]: https://i.imgur.com/FBXzAGH.gif
[piMemeLink]: https://imgur.com/gallery/r102w
[cookArticle]: https://www.johndcook.com/blog/2019/10/22/hacking-with-de-bruijn/
[piNormal]: https://arxiv.org/pdf/1608.00430.pdf
[expectedValue]: https://stats.stackexchange.com/a/13377
[piSource]: https://pi2e.ch/blog/2017/03/10/pi-digits-download/#download
