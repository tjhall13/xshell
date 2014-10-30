#include <xsh_process.h>
#include <sys/types.h>

int main(int argc, char **argv) {

xsh_process_entry w = {0, 1243, RUNNING, FALSE};
if(xsh_create_process_entry(&w)) {
	printf("Successfully added process 1\n");
} else {
	printf("Error on 1");
}

xsh_process_entry x = {0, 1543, RUNNING, FALSE};
if(xsh_create_process_entry(&x)) {
	printf("Successfully added process 2\n");
} else {
	printf("Error on 2");
}

xsh_process_entry y = {0, 1983, BLOCKED, TRUE};
if(xsh_create_process_entry(&y)) {
	printf("Successfully added process 3\n");
} else {
	printf("Error on 3");
}

xsh_process_entry z = {0, 2256, RUNNING, TRUE};
if(xsh_create_process_entry(&z)) {
	printf("Successfully added process 4\n");
} else {
	printf("Error on 4");
}

xsh_process_entry* a = xsh_retrieve_process_entry_by_id(0);
xsh_process_entry* b = xsh_retrieve_process_entry_by_id(1);
xsh_process_entry* c = xsh_retrieve_process_entry_by_id(2);
xsh_process_entry* d = xsh_retrieve_process_entry_by_id(3);


printf("%-10s%-10s%-10s%-10s\n","entry_id","pid","state","fg");
if(a != NULL) {printf("%-10d%-10d%-10d%-10d\n",a->entry_id,a->pid,a->state,a->fg);}
if(b != NULL) {printf("%-10d%-10d%-10d%-10d\n",b->entry_id,b->pid,b->state,b->fg);}
if(c != NULL) {printf("%-10d%-10d%-10d%-10d\n",c->entry_id,c->pid,c->state,c->fg);}
if(d != NULL) {printf("%-10d%-10d%-10d%-10d\n",d->entry_id,d->pid,d->state,d->fg);}

printf("\n\n");

xsh_process_entry* h = xsh_retrieve_process_entry_by_pid(2256);
xsh_process_entry* i = xsh_retrieve_process_entry_by_pid(1543);
xsh_process_entry* j = xsh_retrieve_process_entry_by_pid(1983);
xsh_process_entry* k = xsh_retrieve_process_entry_by_pid(1243);


printf("%-10s%-10s%-10s%-10s\n","entry_id","pid","state","fg");
if(h != NULL) {printf("%-10d%-10d%-10d%-10d\n",h->entry_id,h->pid,h->state,h->fg);}
if(i != NULL) {printf("%-10d%-10d%-10d%-10d\n",i->entry_id,i->pid,i->state,i->fg);}
if(j != NULL) {printf("%-10d%-10d%-10d%-10d\n",j->entry_id,j->pid,j->state,j->fg);}
if(k != NULL) {printf("%-10d%-10d%-10d%-10d\n",k->entry_id,k->pid,k->state,k->fg);}



if(xsh_update_process_entry(2256, STOPPED, FALSE)) {printf("%-10d%-10d%-10d%-10d\n",h->entry_id,h->pid,h->state,h->fg);}

xsh_process_entry* q = xsh_retrieve_foreground_process();
if(q != NULL) {printf("%-10d%-10d%-10d%-10d\n",q->entry_id,q->pid,q->state,q->fg);}

if(xsh_delete_process_entry(1543)) {i=NULL;}
if(xsh_delete_process_entry(1243)) {k=NULL;}

printf("%-10s%-10s%-10s%-10s\n","entry_id","pid","state","fg");
if(h != NULL) {printf("%-10d%-10d%-10d%-10d\n",h->entry_id,h->pid,h->state,h->fg);}
if(i != NULL) {printf("%-10d%-10d%-10d%-10d\n",i->entry_id,i->pid,i->state,i->fg);}
if(j != NULL) {printf("%-10d%-10d%-10d%-10d\n",j->entry_id,j->pid,j->state,j->fg);}
if(k != NULL) {printf("%-10d%-10d%-10d%-10d\n",k->entry_id,k->pid,k->state,k->fg);}


}
