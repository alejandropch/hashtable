#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define HASHTABLE_LENGTH 8

struct hashtable {
  char *key;
  char *value;
};
struct metadata {

  struct hashtable ** ht;
  unsigned int size;
  unsigned int filled;
};

struct hashtable **create_ht()
{
    return malloc(sizeof(struct hashtable *) * HASHTABLE_LENGTH);
}

unsigned int hash(char *key)
{
  unsigned int index = 0;
  for( 
    int i=0; 
    i < strlen(key); 
    index = (key[i]+ index) * key[i], i++
    );
  return index;
}

void reallocate_keys(struct metadata *t)
{
  t->size =t->size * 2;
  struct hashtable **new_ht = malloc( sizeof(struct hashtable *) * t->size);
  
  for(int i = 0; i < (t->size/2); i++)
  {
    if(t->ht[i] == NULL)continue;

    unsigned int new_index = hash(t->ht[i]->key) & (t->size - 1);
    while(new_ht[new_index] != NULL) new_index = (new_index + 1) % t->size;

    new_ht[new_index] = malloc(sizeof(struct hashtable));
    new_ht[new_index] =  t->ht[i];
  }
  free(t->ht);
  t->ht = new_ht;
} 

void insert(struct metadata *t, char *key, char *value)
{
  /* realloc memory if necessary */
  if((float) t->filled / (float)t->size > 0.5) {
  printf("filled: %d\t size: %d\tfloat: %f\n", t->filled, t->size, (float) t->filled / (float)t->size);
      reallocate_keys(t);
  }

  for(
      unsigned int index = hash(key) &( t->size - 1);
      ;
      index= (index + 1) %t->size
    )
    {
      if(t->ht[index] == NULL) {

        t->ht[index] = malloc(sizeof(struct hashtable));
        t->ht[index]->key = calloc(strlen(key) + 1, sizeof(key[0]));
        t->ht[index]->value = calloc(strlen(value) + 1, sizeof(value[0]));
        strncpy(t->ht[index]->key, key, strlen(key) + 1);
        strncpy(t->ht[index]->value, value, strlen(value) + 1);
        t->filled++;
        break; 
      }else if (strcmp(t->ht[index]->key, key) == 0){
        free(t->ht[index]->value);
        t->ht[index]->value = malloc(strlen(value) + 1);
        strncpy(t->ht[index]->value, value, strlen(value) + 1);
        break;
      };
    }
}

void* get(struct metadata *t, char *key )
{  
  for(
      unsigned int index = hash(key) &( t->size - 1);
      ;
      index= (index + 1) %t->size
    )
    {
      if(t->ht[index] == NULL){
        printf("there is no item with the key: %s\n", key);
        return NULL;
      }
      if( strcmp(t->ht[index]->key, key) == 0 )
        return t->ht[index]->value;
    }
}

void print_ht(struct metadata *t)
{
  for(int i = 0; i < t->size; i++){
    printf("%d\t %p\t", i, t->ht[i]);

    if (t->ht[i] == NULL) puts("-");
    else printf("%s\n",t->ht[i]->value);
  }
}

void delete_item(struct metadata *t, char *key)
{
  for(
    unsigned int index = hash(key) &( t->size - 1); 
    ; 
    index= (index + 1) %t->size
    )
  {
    printf("key: %u\n", index);

    if (t->ht[index] == NULL)
      break;
    else if( strncmp(t->ht[index]->key, key, strlen(key) + 1) == 0){

      free(t->ht[index]->key);
      free(t->ht[index]->value);
      free(t->ht[index]);
      t->ht[index] = NULL;
      t->filled--;
    }
  }
}


int main(void){
  struct metadata table = {.size = HASHTABLE_LENGTH, .filled = 0,.ht = create_ht()};

  insert(&table, "index1", "this is a cool 1");
  insert(&table, "index2", "this is a cool 2");
  insert(&table, "index3", "this is a cool 3");
  insert(&table, "index4", "this is a cool 4");
  insert(&table, "index5", "this is a cool 5");
  insert(&table, "index6", "this is a cool 6");
  insert(&table, "index7", "this is a cool 7");
  insert(&table, "index8", "this is a cool 8");
  insert(&table, "index9", "this is a cool 9");
  insert(&table, "index10", "this is a cool 10");
  insert(&table, "index11", "this is a cool 11");
  insert(&table, "index12", "this is a cool 12");
  insert(&table, "index13", "this is a cool 13");
  insert(&table, "index14", "this is a cool 14");
  insert(&table, "index15", "this is a cool 15");

  print_ht(&table); 
  delete_item(&table, "index1");
  puts("----------------");
  print_ht(&table); 
  char *s = get(&table, "index15");
  if(s != NULL) printf("%s\n", s);
  return 0;
}