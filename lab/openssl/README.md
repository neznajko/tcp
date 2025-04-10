# openssl

So I'm reading here one book;
*"Demystifying Cryptography with OpenSSL 3.0"*, and trying to figure
out what's going on.

# *abstract algebra*

### 11. Prove the following version of the Division Algorithm, which holds for both positive and negative divisors.

*Extended Division Algorithm: Let **a** and **b** be integers with
**b ≠ 0**. Then there exist unique integers **q** and **r** such that
**a = bq + r** and **0 ≤ r < |b|**.*

```bash
Let's divide a by |b|, then:
a = q|b| + r, 0 <= r < |b|

a) b > 0, then |b| = b, and
a = qb + r, 0 <= r < |b|

b) b < 0, then |b| = -b, and
a = -qb + r = (-q)b + r, 0 <= r < |b|
```