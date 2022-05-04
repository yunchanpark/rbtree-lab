#include "rbtree.h"
#include <stdlib.h>

// 트리 초기화
rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));			  // 트리 생성
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));	// 경계 노드 생성
  nil_node->color = RBTREE_BLACK;							            // 경계 노드 색(검정)
  t->nil = nil_node;										                  // 트리의 경계 노드는 생성한 경계 노드 
  t->root = nil_node;										                  // 트리의 루트 노드는 생성한 경계 노드
  return t;													                      // 트리 반환
}

// 각각의 노드들이 가리키는 공간들 해제
void delete_node(node_t *node, rbtree *t) {
  // 현재 노드가 트리의 nil노드와 같으면 그냥 return
  if (node == t->nil) {
    return;
  }

  delete_node(node->left, t);   // 왼쪽으로 재귀
  delete_node(node->right, t);  // 오른쪽으로 재귀
  free(node);                   // 현재 노드가 가리키는 공간 할당 해제
  node = NULL;                  // 할당 해제 후 현재 노드값을 NULL로 초기화
  return;
}

// 트리, 트리의 nil이 가리키는 공간 해제
void delete_rbtree(rbtree *t) {
  // 트리가 없으면 return
  if (t == NULL) {
    return;
  }

  delete_node(t->root, t);  // 생성된 노드들이 가리키는 공간 할당 해제
  free(t->nil);             // 트리의 nil노드가 가리키는 공간 할당 해제
  t->nil = NULL;            // 할당 해제 후 트리의 nil노드값을 NULL로 초기화
  free(t);                  // 트리가 가르키는 공간 할당 해제
  t = NULL;                 // 할당 해제 후 트리의 값을 NULL로 초기화
  return;
}

// 좌회전
void left_rotation(rbtree* t, node_t* x) {
  node_t* y = x->right;               // y = 현재 노드의 오른쪽
  x->right = y->left;                 // x의 오른쪽 자식을 y의 왼쪽 자식으로 변경
  if (y->left != t->nil) {            // 만약 y의 왼쪽이 nil이 아니면
    y->left->parent = x;              // y의 왼쪽 부모을 x로 변경              
  }
  y->parent = x->parent;              // y의 부모를 x의 부모로 변경
  if (x->parent == t->nil) {          // x의 부모가 nil이라면 루트라는 뜻
    t->root = y;                      // 트리의 루트를 y로 변경
  } else if (x == x->parent->left) {  // x가 x의 부모의 왼쪽 자식이라면
    x->parent->left = y;              // x의 부모의 왼쪽을 y로 변경
  } else {                            // x가 x의 부모의 오른쪽 자식이라면
    x->parent->right = y;             // x의 부모의 오른쪽을 y로 변경
  }
  y->left = x;                        // y의 왼쪽을 x로 변경
  x->parent = y;                      // x의 부모를 y로 변경
  return;
}

// 우회전
// 좌회전과 반대로 하면 됨
void right_rotation(rbtree* t, node_t* x) {
  node_t* y = x->left;
  x->left = y->right;
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
  return;
}

// 색 변경
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {      // z의 부모가 RED일때
    if (z->parent == z->parent->parent->left) { // z의 부모가 z의 조상의 왼쪽 자식이라면
      node_t *y = z->parent->parent->right;     // y는 z의 조상의 오른쪽 자식(삼촌)
      if (y->color == RBTREE_RED) {             // 경우 1. z의 삼촌의 색이 빨강일때
        z->parent->color = RBTREE_BLACK;          // z의 부모 색을 BLACK으로 변경
        y->color = RBTREE_BLACK;                  // z의 삼촌의 색을 BLACK으로 변경 
        z->parent->parent->color = RBTREE_RED;    // z의 조상의 색을 RED로 변경 
        z = z->parent->parent;                    // z는 조상으로 변경
      } else {                                  // 경우 2. z의 삼촌의 색이 검정일때
          if (z == z->parent->right) {            // z가 z의 부모의 오른쪽 자식일때
            z = z->parent;                        // z는 z의 부모로 변경
            left_rotation(t, z);                  // z를 기준으로 좌회전
        }                                       // 경우 3. z의 삼촌의 색이 검정이고 z가 z의 부모의 왼쪽 자식일 때
        z->parent->color = RBTREE_BLACK;          // z의 부모 색을 검정으로 변경
        z->parent->parent->color = RBTREE_RED;    // z의 조상의 색을 레드로 변경
        right_rotation(t, z->parent->parent);     // z의 조상 기준으로 우회전
      }
    } else {                                    // z의 부모가 z의 조상의 오른쪽이라면 왼쪽과 반대로 함
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
          if (z == z->parent->left) {
            z = z->parent;
            right_rotation(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotation(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;                // 특성 2을 유지하기 위해 root는 항상 BLACK으로 설정
  return;
}

// 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t* y = t->nil;     // y는 트리의 nil노드
  node_t* x = t->root;    // x는 트리의 root노드
  while (x != t->nil) {   // x가 트리의 nil노드가 아닐 때 반복(즉, x의 자식들이 leaf노드에 도달할 때 까지)
    y = x;                // y는 x
    if (key < x->key) {   // 만약 x의 key값보다 삽입할 key값이 작으면
      x = x->left;        // x는 x의 왼쪽으로 변경
    } else {              // 만약 x의 key값보다 삽입할 key값이 크거나 같으면
      x = x->right;       // x는 x의 오른쪽으로 변경
    }
  }
  node_t* z = (node_t *)calloc(1, sizeof(node_t));  // z(노드) 생성
  z->parent = y;              // z의 부모는 y
  if (y == t->nil) {          // y가 트리의 nil일 때(즉, 첫 노드 삽입)
    t->root = z;              // 트리의 root는 z
  } else if (key < y->key) {  // y의 key값이 삽입할 키 값보다 작을 때
    y->left = z;              // y의 왼쪽 자식은 z
  } else {                    // y의 key값이 삽입할 키 값보다 크거나 같을 때
    y->right = z;             // y의 오른쪽 자식은 z
  }
  z->key = key;               // z의 key값은 현재 key
  z->color = RBTREE_RED;      // z의 color값은 RED, 삽입할 때는 무조건 RED
  z->left = t->nil;           // z의 왼쪽 자식은 트리의 nill, 삽입할 때는 무조건 자식은 nill
  z->right = t->nil;          // z의 왼쪽 자식은 트리의 nill, 삽입할 때는 무조건 자식은 nill
  rbtree_insert_fixup(t, z);  // fixup 호출
  return t->root;             // 트리의 root값 반환
}

// 트리에서 원하는 값 찾기
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root;                    // x는 트리의 루트
  while (x != t->nil && key != x->key) {  // x가 nil노드가 아니거나 찾는 key값이 x의 key 값이 아닐 때 반복
    if (x->key < key) {                   // 찾는 key 값이 x의 key 값보다 클 때
      x = x->right;                       // x는 x의 오른쪽 자식
    } else {                              // 찾는 key 값이 x의 key 값보다 작을 때
      x = x->left;                        // x는 x의 왼쪽 자식
    }
  }
  if (x == t->nil) {                      // x가 nill노드면, 즉 key를 찾지 못했을 때
    return NULL;                          // NULL 반환
  }
  return x;                               // x가 nil노드가 아니면 x 반환, 즉 key를 찾았을 때
}

// 트리의 최솟값
node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;        // x는 트리의 루트
  while (x->left != t->nil) { // x의 왼쪽 자식이 nil노드가 아닐 때
    x = x->left;              // x는 x의 왼쪽 자식
  }
  return x;                   // x 반환
}

// 트리의 최댓값
node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;          // x는 트리의 루트
  while (x->right != t->nil) {  // x의 오른쪽 자식이 nil노드가 아닐 때
    x = x->right;               // x는 x의 오른쪽 자식
  }
  return x;                     // x 반환
}

// u의 부모와 v와 연결
void rbtree_transplant(rbtree *t, node_t *u, node_t * v) {
  if (u->parent == t->nil) {          // u의 부모가 nil일 때, 즉, 삭제할 노드가 트리의 root 일때
    t->root = v;                      // 트리의 root는 v
  } else if (u == u->parent->left) {  // u가 u의 부모의 왼쪽 자식일 때 
    u->parent->left = v;              // u의 부모의 왼쪽 자식 v
  } else {                            // u가 u의 부모의 오른쪽 자식일 때
    u->parent->right = v;             // u의 부모의 오른쪽 자식은 v
  }
  v->parent = u->parent;              // v의 부모는 u의 부모
  return;
}

void rb_delete_fixup(rbtree *t, node_t *x){
    node_t *w;
    while ((x != t->root) && (x->color == RBTREE_BLACK)) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotation(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            }  else {
                if (w->right->color == RBTREE_BLACK) {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotation(t, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotation(t, x->parent);
                x = t->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotation(t, x->parent);
                w = x ->parent->left;
            }
            if (w->right ->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotation(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left -> color = RBTREE_BLACK;
                right_rotation(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
    return;
}

// 직후 원소 찾기, x는 삭제할 노드의 오른쪽 자식
node_t *rbtree_successor(rbtree *t, node_t *x) {
  node_t *y = x;              // y는 x
  while (y->left != t->nil) { // y의 왼쪽 자식이 nil이 아닐 때 반복
    y = y->left;              // y는 y의 왼쪽 자식
  }
  return y;                   // y를 반환
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *x;                              // 노드 x
  node_t *y = p;                          // y는 삭제할 노드
  color_t y_color = y->color;             // y_color는 y의 색
  if (p->left == t->nil) {                // 삭제할 노드의 왼쪽이 nil일때
    x = p->right;                         // x는 삭제할 노드의 오른쪽
    rbtree_transplant(t, p, p->right);    // 삭제할 노드의 부모와 삭제할 노드의 오른쪽을 연결 
  } else if (p->right == t->nil) {        // 삭제할 노드의 오른쪽이 nill일때
    x = p->left;                          // x는 삭제할 노드의 왼쪽
    rbtree_transplant(t, p, p->left);     // 삭제할 노드의 부모와 삭제할 노드의 왼쪽을 연결
  } else {                                // 삭제할 노드의 왼쪽, 오른쪽 자식이 둘다 있을 때
    y = rbtree_successor(t, p->right);    // y는 직후 원소
    y_color = y->color;                   // y_color는 직후 원소의 색
    x = y->right;                         // x는 y의 오른쪽 자식
    if (y->parent == p) {                 // y의 부모가 삭제할 노드일 때
      x->parent = y;                      // x의 부모는 y
    } else {                              // y의 부모가 삭제할 노드가 아닐 때
      rbtree_transplant(t, y, y->right);  // y의 부모와 y의 오른쪽 자식을 연결
      y->right = p->right;                // y의 오른쪽 자식은 삭제할 노드의 오른쪽 자식
      y->right->parent = y;               // y의 오른쪽 자식의 부모는 y
    }
    rbtree_transplant(t, p, y);           // 삭제할 노드 부모와 y를 연결
    y->left = p->left;                    // y의 왼쪽 자식은 삭제할 노드의 왼쪽 자식
    y->left->parent = y;                  // y의 왼쪽 자식의 부모는 y
    y->color = p->color;                  // y의 색은 삭제할 노드의 색
  }
  free(p);                                // 삭제한 노드가 가리키는 공간 삭제
  p = NULL;                               // 할당 해제 후 삭제한 노드값을 NULL로 초기화
  if (y_color == RBTREE_BLACK) {          // y_color가 BLACK 일 때, 즉 삭제한 노드의 색이 BLACK일 때(특성 5 위반)
    rb_delete_fixup(t, x);                // 노드의 색을 바꿈
  }
  return 0;
}

// 트리의 중위 순회
// root 노드는 계속 변화
int rbtree_inorder(node_t *nil, node_t *root, key_t *arr, const size_t n, int index) {
  if (root == nil) {                                        // root노드가 nil노드일 때
    return index;                                           // index 반환
  }
  if (index == n) {                                         // index가 n하고 같을 때 더 이상 재귀 돌지 않게 종료 조건
    return index;                                           // index반환
  } 
  index = rbtree_inorder(nil, root->left, arr, n, index);   // root노드를 root노드의 왼쪽 자식으로 재귀
  if(index < n) {                                           // idenx가 n보다 작을 때만 arr에 추가
    arr[index++] = root->key;                               // arr[현재 index] = 현재 노드의 키 값 
  }
  index = rbtree_inorder(nil, root->right, arr, n, index);  // root노드를 root노드의 오른쪽 자식으로 재귀
  return index;                                             // index 반환
}

// 트리를 이용하여 오름차순 구현
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // 트리의 루트가 nil노드일 때 반환
  if (t->root == t->nil) {
    return 0;
  }
  rbtree_inorder(t->nil, t->root, arr, n, 0); // 중위 순회
  return 0;
}