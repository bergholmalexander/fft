// Attempt at solving https://codeforces.com/contest/528/problem/D
// Citations: reverse, fft and multiply from https://cp-algorithms.com/algebra/fft.html as I ran out of time
#include <bits/stdc++.h>
using namespace std;
int S; int T; int k;

using cd = complex<double>;
const double PI = acos(-1);

int reverse(int num, int lg_n) {
    int res = 0;
    for (int i = 0; i < lg_n; i++) {
        if (num & (1 << i))
            res |= 1 << (lg_n - 1 - i);
    }
    return res;
}

void fft(vector<cd> & a, bool invert) {
    int n = a.size();
    int lg_n = 0;
    while ((1 << lg_n) < n)
        lg_n++;

    for (int i = 0; i < n; i++) {
        if (i < reverse(i, lg_n))
            swap(a[i], a[reverse(i, lg_n)]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i+j], v = a[i+j+len/2] * w;
                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd & x : a)
            x /= n;
    }
}

vector<int> multiply(vector<int> const& a, vector<int> const& b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size())
        n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
    fft(fa, true);

    vector<int> result(n);
    for (int i = 0; i < n; i++)
        result[i] = round(fa[i].real());
    return result;
}

vector<int> bitmask(char s[], char a) {
  // C Bitmask
  vector<int> c(S,0);
  for (int i=0; i<S; ++i) {
    if (s[i] == a) {
      c[max(i-k, 0)] += 1;
      if (i+k+1 < S) {
        c[i+k+1] -= 1;
      }
    }
  }
  for (int i=1; i<S; i++) {
    c[i] += c[i-1];
  }
  for (int i=1; i<S; i++) {
    c[i] = c[i] > 0 ? 1 : 0; // todo: check correctness
  }
  return c;
}
vector<int> Tbitmask(char t[], char a) {
  // T's Bitmask
  vector<int> c(T,0);
  for (int i=0; i<T; ++i) {
    if (t[i] == a) {
      c[i] = 1;
    }
  }
  reverse(c.begin(), c.end());
  return c;
}

int main(){
  // Input
  // |S|, |T|, k
  scanf("%d%d%d",&S,&T,&k);
  vector<int> matches(S,0);
  char s[S];
  char t[T];
  scanf("%s%s",&s,&t); // String S, T containing only characters 'A', 'T', 'G', 'C'
  // For each character (A,T,G,C) create a bitvector spanning left and right from each letter by k.
  // A Bitmasks
  vector<int> a = bitmask(s, 'A'); // 1 where s[i] = A +/- k
  vector<int> Ta = Tbitmask(t, 'A'); // 1 where t[i] = A
  vector<int> res1 = multiply(a, Ta);
  // T Bitmasks
  vector<int> tarr = bitmask(s, 'T');
  vector<int> Tt = Tbitmask(t, 'T');
  vector<int> res2 = multiply(tarr, Tt);
  // G Bitmasks
  vector<int> g = bitmask(s, 'G');
  vector<int> Tg = Tbitmask(t, 'G');
  vector<int> res3 = multiply(g, Tg);
  // C Bitmasks
  vector<int> c = bitmask(s, 'C');
  vector<int> Tc = Tbitmask(t, 'C');
  vector<int> res4 = multiply(c, Tc);
  // Count #scenarios in which result == |T|
  int count = 0;
  for(int i=0; i<S; i++) {
    if (res1[i]+res2[i]+res3[i]+res4[i] == T) {
      count++;
    }
  }
  printf("%d",count);
	return 0;
}
