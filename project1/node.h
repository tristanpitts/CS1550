struct node{
  int data; //process id
  struct node *next;
  //pop function gets data from the head node
  //and moves the head to the next node
  //
  //push function adds a new node at the end
}

int pop(sem *s)
{
  int psid = s->head->data;
  struct node *temp = s->head;
  s->head = s->head->next;
  kfree(temp);
  return psid;
}

void push(node *s, int psid)
{
  s->tail->next = kmalloc(sizeof(struct node));
}
