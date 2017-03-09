#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "main.h"

int bpowe(int b, int e) {
  int o = 1;
  int i;
  for (i = 0; i < e; i++) {
    o *= b;
  }
  return o;
}

int find_ngram(char buf[], ngram_t target[]) {
  if (DEBUGGING > 5) printf("find_ngram: start\n");
  int i;
  for (i = 0; i < bpowe(27, N); i++) {
    if (!strncmp(target[i].ngram, buf, N)) {
      if (DEBUGGING > 5) printf("analyse: end +ve\n");
      return i;
    }
  }
  if (DEBUGGING > 5) printf("analyse: end -ve\n");
  return -1;
}

// void old_init_ngrams(ngram_t target[]) {
//   int c, i, n;
//   for (c = 0; c < bpowe(27, N); c++) {
//     for (i = 0; i < N ; i++) {
//       n = c - (c % bpowe(27, N - i - 1));
//       n /= bpowe(N, N - i - 1);
//       target[c].ngram[i] = letters[n];
//     }
//   }
// }

void init_ngrams(ngram_t target[]) {
  int i;
  int x = 0;
  int y = 0;
  int z = 0;

  for (i = 0; i < bpowe(27, N); i++) {
    target[i].val = 0;
    target[i].ngram[0] = letters[x];
    target[i].ngram[1] = letters[y];
    target[i].ngram[2] = letters[z];
    z++;
    if (z >= 27) {
      z = 0;
      y += 1;
      if (y >= 27) {
        y = 0;
        x++;
      }
    }
    if (x == y && y == z && z == 27) {
      break;
    }
  }
}

void print_ngrams(ngram_t target[]) {
  int i;
  char curngram[N+1];
  curngram[N] = '\0';
  for (i = 0; i < bpowe(27, N); i++) {
    if (target[i].val > 0) {
      snprintf(curngram, N+1, "%s", target[i].ngram);
      printf("%s: %d\t", curngram, target[i].val);
    }
  }
  printf("\n");
}

void test_init(ngram_t target[]) {
  int c;
  char curngram[N+1];
  printf("ngrams: ");
  for (c = 0; c < 5; c++) {
    snprintf(curngram, N+1, "%s", target[c].ngram);
    printf("%s ", curngram);
  }
  printf("\n");
}

void read_dict(char *filename, char target[NUM_WORDS][MAX_WORD_LEN+1]) {
  FILE *fptr = NULL;
  int retval, i;
  int curword = 0;
  char buf[MAX_WORD_LEN+1];

  fptr = fopen(filename,"r");
  if(fptr == NULL) {
    perror("read_dict: file did not open successfully");
    exit(EXIT_FAILURE);
  }

  retval = fscanf(fptr, "%s", buf);
  if (retval <= 0) {
    perror("read_dict: could not read first word");
    exit(EXIT_FAILURE);
  } else {
    // if (DEBUGGING) printf("%s\n", buf);
  }
  strcpy(target[curword], buf);
  memset(&buf[0], 0, sizeof(buf));

  for (i = 0; i < NUM_WORDS; i++) {
    retval = fscanf(fptr, "%s*[\r\n]", buf);
    if (retval == EOF) {
      break;
    } else if (retval < 0) {
      perror("read_dict: fscanf returned < 0");
    } else {
      // if (DEBUGGING) printf("%s\n", buf);
    }
    curword++;
    strcpy(target[curword], buf);
    memset(&buf[0], 0, sizeof(buf));
  }
}

// note: only pass in LENPT+1 long arrays to target
void gen_sample_pt(char source[NUM_WORDS][MAX_WORD_LEN+1], char *target) {
  // if (DEBUGGING > 5) printf("gen_sample_pt: start\n");
  int curaddr = 0;
  int randword;
  int wordlen;
  srand(time(NULL)); //seed rand()
  while (LENPT - curaddr - 1 > 0) {
    // if (DEBUGGING > 5) printf("rand() = %d\n", rand());
    randword = rand() % NUM_WORDS;
    // if (DEBUGGING > 5) printf("gen_sample_pt: word#: %d\n", randword);
    // if (DEBUGGING > 5) printf("gen_sample_pt: word: %s\n", source[randword]);
    wordlen = strnlen(source[randword], LENPT - curaddr);
    // if (DEBUGGING > 5) printf("gen_sample_pt: strncpy word\n");
    if (strncpy(&target[curaddr], source[randword], wordlen) == NULL) {
      perror("gen_sample: strncpy failed");
    }
    if (curaddr < LENPT) {
      target[curaddr+wordlen] = ' ';
    }
    curaddr += wordlen + 1;
  }
  // if (DEBUGGING > 5) printf("gen_sample_pt: end\n");
}

void find_base_score(char source[NUM_WORDS][MAX_WORD_LEN+1], ngram_t target[], score_t score_dest[]) {
  score_t scores[NTESTS];
  int i, j, avgscore, avgwlen;
  score_t curscore, lowscore;
  char curpt[NTESTS][LENPT+1];
  printf("find_base_score: gen pts and analyse\n");
  // generate plaintexts and map their ngrams
  for (i = 0; i < NTESTS; i++) {
    // for (j = 0; j < bpowe(27, N); j++) {
      gen_sample_pt(source, curpt[i]);
      // if (DEBUGGING > 1) printf("%s\n\n", curpt[i]);
      analyse(curpt[i], LENPT, target);
    // }
  }

  printf("find_base_score: calc scores and add to map\n");
  // calculate the scores for each one from the cumulative map
  for (i = 0; i < NTESTS; i++) {
    curscore = score(curpt[i], LENPT, target);
    scores[i] = curscore;
  }

  printf("find_base_score: find avgs\n");
  // find avgscore, avgwlen, and lowest score
  lowscore = scores[0];
  for (i = 0; i < NTESTS; i++) {
    avgscore += scores[i].score;
    avgwlen += scores[i].avgwlen;
    if (scores[i].score < lowscore.score) {
      lowscore = scores[i];
    }
  }
  avgscore /= NTESTS;
  avgwlen /= NTESTS;
  gbavgscore.score = avgscore;
  gbavgscore.avgwlen = avgwlen;
  gblowscore = lowscore;
  printf("find_base_score: end\n");
  return;
}

score_t score(char *buf, int len, ngram_t sample[]) {
  int i, t, nwords = 0, curwlen = 2;
  int wlens[len];
  score_t out;
  out.score = 0;
  out.avgwlen = 0;
  char curngram[N+1];
  curngram[N] = '\0';

  for (i = 0; i < len + 1 - N; i++) {
    snprintf(curngram, N+1, "%s", &buf[i]);
    // if (DEBUGGING > 1) printf("curngram: %s\n", curngram);
    if ((t = find_ngram(curngram, sample)) < 0) {
      out.score += 0;
    } else {
      out.score += sample[t].val;
    }
    if (curngram[N-1] == ' ') {
      wlens[nwords] = curwlen;
      curwlen = 0;
      nwords++;
    }
  }
  i = 0;
  for (i = 0; i < nwords; i++) {
    curwlen = wlens[i];
    out.avgwlen += curwlen;
  }
  out.avgwlen /= (nwords+1);
  return out;
}

// void ngram_add(ngram_t source[], ngram_t target[]) {
//   int i;
//   for (i = 0; i < bpowe(27, N); i++) {
//     target[i].val += source[i].val;
//   }
// }

int analyse(char *buf, int len, ngram_t target[]) {
  // if (DEBUGGING > 5) printf("analyse: start\n");
  int i, j, t;
  char curngram[N+1];
  curngram[N] = '\0';

  for (i = 0; i < len + 1 - N; i++) {
    snprintf(curngram, N+1, "%s", &buf[i]);
    // if (DEBUGGING > 5) printf("curngram: %s\n", curngram);
    if ((t = find_ngram(curngram, target)) < 0) {
      if (DEBUGGING > 5) printf("analyse: ngram not found: %s\n", curngram);
    }
    target[t].val++;
  }

  if (DEBUGGING > 5) print_ngrams(target);
  // if (DEBUGGING > 5) printf("analyse: end\n");
  return EXIT_SUCCESS;
}
