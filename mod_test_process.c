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


printf("%s\t%s\t%s\t%s\n","entry_id","pid","state","fg");
printf("%d\t%d\t%d\t%d\n",a->entry_id,a->pid,a->state,a->fg);
printf("%d\t%d\t%d\t%d\n",b->entry_id,b->pid,b->state,b->fg);
printf("%d\t%d\t%d\t%d\n",c->entry_id,c->pid,c->state,c->fg);
printf("%d\t%d\t%d\t%d\n",d->entry_id,d->pid,d->state,d->fg);




}
