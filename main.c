#include "helpers.c"

char dict[NUM_WORDS][MAX_WORD_LEN+1];

int main () {
  int pown = bpowe(27, N);
  int i;
  ngram_t pt_ngrams[pown];
  ngram_t sm_ngrams[pown];
  char en_dict_fn[18] = "english_words.txt";
  char pt_dict_fn[25] = "plaintext_dictionary.txt";

  char plaintext[LENPT+1] = "cabooses meltdowns bigmouth makework flippest \
neutralizers gipped mule antithetical imperials carom masochism honky retsina \
dullness adeste corsage saraband promenaders gestational mansuetude fig \
redress pregame borshts pardoner reforges refutations calendal moaning \
doggerel dendrology governs ribonucleic circumscriptions reassimilating \
machinize rebuilding mezcal fluoresced antepenults blacksmith constance \
furores chroniclers overlie hoers jabbing resigner quartics polishers mallow \
hovelling ch";

  score_t base_scores[2];

  srand(time(NULL)); //seed rand()
  if (DEBUGGING > 0) printf("rand() = %d\n", rand());

  init_ngrams(pt_ngrams);
  init_ngrams(sm_ngrams);
  if (DEBUGGING) test_init(pt_ngrams);

  read_dict(en_dict_fn, dict);
  if (DEBUGGING) {
    printf("dict-words: ");
    for (i = 0; i < 5; i++) {
      printf("%d: %s, ", i+1, dict[i]);
    }
    printf("\n");
  }


  // int n = analyse(plaintext, LENPT, pt_ngrams);
  srand(time(NULL)); //seed rand()
  find_base_score(dict, sm_ngrams, base_scores);
  printf("main: find_base_score: returned\n");

  if (DEBUGGING) printf("main: end. \n");
  return 0;
}
