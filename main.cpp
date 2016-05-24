#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
using namespace std;

struct sMzi{
	int znak;
	int wys;
	bool operator<(sMzi b)const {
		return wys > b.wys;
	}
};

struct sTdk {
	int ojc, p0, p1;
};

struct sTk {
	int sym, dlu;
	string kod;
};

char wBuff[256];

void boolToByte(bool *arr, int size) {
	char val;
	for (int i = 0; i < size; i++) {
		val = 0;
		for (int j = 0; j < 8; j++) {
			val <<= 1;
			val |= arr[8*i + j];
		}
		wBuff[i] = val;
	}
}

int main() {
	const int tRoz = 128;
	sMzi tWys[tRoz] = {0}, tmpMzi[tRoz];
	sTdk tTdk[tRoz - 1] = { 0 };
	sTk tTk[tRoz], tmpTk[tRoz], *sTmp;
	FILE *fIn, *fOut;
	fIn = fopen("plik.txt", "rt");
	int c, i, n, p;
	while ((c = fgetc(fIn)) != EOF) {
		tWys[c].znak = c;
		tWys[c].wys++;
	}
	fclose(fIn);
	sort(tWys, tWys + tRoz);
	fOut = fopen("mzi.txt", "wt");
	i = 0;
	while (tWys[i].wys != 0 && i < tRoz) {
		fprintf(fOut, "%d\t%c\t%d\n", i, tWys[i].znak, tWys[i].wys);
		i++;
	}
	n = i;
	fclose(fOut);
	fIn = fopen("mzi.txt", "rt");
	sMzi tmp;
	for (int j = 0; j < n; j++) {
		p = 0;
		while ((c = getc(fIn)) != 10) {
			if (c != '\t') {
				if (p == 1)
					tmp.znak = c;
				else if(p==2)
					tmp.wys = c - '0';
				p++;
			}
		}
		tmpMzi[j] = tmp;
	}
	i = n-1;
	p = 0;
	int tmpn = n;
	while (i >= 1) {
		tTdk[p].ojc = tmpn*(-1);
		tTdk[p].p0 = tmpMzi[i].znak;
		tTdk[p].p1 = tmpMzi[i - 1].znak;
		tmpMzi[i - 1].wys += tmpMzi[i].wys;
		tmpMzi[i - 1].znak = tTdk[p].ojc;
		i--;
		p++;
		tmpn++;
		sort(tmpMzi, tmpMzi + i);
	}
	fOut = fopen("tdk.txt", "wt");
	for (int j = 0; j < n - 1; j++) {
		if (tTdk[j].p0 >= 0 && tTdk[j].p1 >= 0)
			fprintf(fOut, "%d\t%c\t%c\n", tTdk[j].ojc, tTdk[j].p0, tTdk[j].p1);
		else if (tTdk[j].p0 >= 0 && tTdk[j].p1 <= 0)
			fprintf(fOut, "%d\t%c\t%d\n", tTdk[j].ojc, tTdk[j].p0, tTdk[j].p1);
		else if (tTdk[j].p0 <= 0 && tTdk[j].p1 >= 0)
			fprintf(fOut, "%d\t%d\t%c\n", tTdk[j].ojc, tTdk[j].p0, tTdk[j].p1);
		else
			fprintf(fOut, "%d\t%d\t%d\n", tTdk[j].ojc, tTdk[j].p0, tTdk[j].p1);
	}
	fclose(fOut);
	i = 0;
	sTmp = &tmpTk[0 - n - tTdk[n - 2].ojc];
	sTmp->kod.clear();
	sTmp->sym = tTdk[n - 1].ojc;
	sTmp->dlu = sTmp->kod.size();
	for (int j = n-2; j >= 0; j--) {
		if (tTdk[j].p0 >= 0) {
			tTk[i].sym = tTdk[j].p0;
			tTk[i].kod = tmpTk[0 - n - tTdk[j].ojc].kod + "0";
			tTk[i].dlu = tTk[i].kod.size();
			i++;
		}
		if (tTdk[j].p1 >= 0) {
			tTk[i].sym = tTdk[j].p1;
			tTk[i].kod = tmpTk[0 - n - tTdk[j].ojc].kod + "1";
			tTk[i].dlu = tTk[i].kod.size();
			i++;
		}
		if (tTdk[j].p0 < 0) {
			tmpTk[j-1].sym = tTdk[j].p0;
			tmpTk[j-1].kod = tmpTk[0 - n - tTdk[j].ojc].kod + "0";
			tmpTk[j-1].dlu = tmpTk[i].kod.size();
		}
		if (tTdk[j].p1 < 0) {
			tmpTk[j-1].sym = tTdk[j].p1;
			tmpTk[j-1].kod = tmpTk[0 - n - tTdk[j].ojc].kod + "1";
			tmpTk[j-1].dlu = tmpTk[i].kod.size();
		}
	}
	fOut = fopen("tk.txt", "wt");
	for (i = 0; i < n; i++) {
		fprintf(fOut, "%c %s %d\n", tTk[i].sym, tTk[i].kod.c_str(), tTk[i].dlu);
	}
	fclose(fOut);
	fIn = fopen("plik.txt", "rt");
	fOut = fopen("hoff.txt", "wb");
	bool buff[256];
	int off = 0;
	while ((c = fgetc(fIn)) != EOF) {
		int f;
		for (i = 0; i < n; i++) {
			if (tTk[i].sym == c) {
				f = i;
				break;
			}
		}
		for (i = 0; i < tTk[f].dlu; i++) {
			buff[off + i] = bool(tTk[f].kod[i] - '0');
		}
		off += tTk[f].dlu;
	}
	int wyp=8;
	while (off > wyp) wyp += 8;
	for (i = 0; i < wyp-off; i++) {
			buff[off + i] = 0;
	}
	int s = wyp/ 8;
	unsigned char pom[1];
	pom[0] = (char)(wyp - off);
	boolToByte(buff, s);
	fwrite(wBuff, 1, s, fOut);
	fwrite(pom, 1, 1, fOut);
	fclose(fOut);
	fclose(fIn);
	return 0;
}