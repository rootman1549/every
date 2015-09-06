#include<stdio.h>
#include<stdlib.h>
#include "../app/listlist.h"

#define STUDENT_FREE_MEMORY

//��������ڵ�
typedef struct {
	int id;
	char *name;
	struct listnode _list;
}student;

//��������ָ��
typedef void (*student_foreach_fun)(student *stu,void *data);


//��������
static list_declare(student_list);

//��ӽڵ�
int student_add(struct listnode *list,student *stu)
{
	list_init(&stu->_list);
	list_add_tail(list,&stu->_list);	
}

//ɾ���ڵ㣬�ͷŽڵ�ռ�
int student_del(struct listnode *list,int id)
{
	struct listnode *node;
	student *stu;
	list_for_each(node,list){
		stu = node_to_item(node,student,_list);
		if(id == stu->id){
			printf("list_del, id:%d,name:%s\n",stu->id,stu->name);
			list_remove(node);
#ifdef STUDENT_FREE_MEMORY	
			//�ͷŽڵ�ռ�
			free(stu);
			stu = NULL;
#endif
			return 1;
			
		}
		
	}

	return 0;
}

//�ڵ����
void student_foreach(struct listnode *list,student_foreach_fun fun,void *data)
{
	struct listnode *node;
	student *stu;
	list_for_each(node,list){
		stu = node_to_item(node,student,_list);
		fun(stu,data);
	}

}

//��ӡ�ڵ���Ϣ
void student_print(student *stu,void *data)
{
	printf("id:%d,name:%s\n",stu->id,stu->name);
}

int main()
{
	int i,len;
	student *stu;
	char *stu_name[]={"tonny","andy","michael","leslie","john"};
	
	
	len = sizeof(stu_name)/sizeof(stu_name[0]);
	//��ӽڵ�
	for(i=0;i<len;i++){
		stu = calloc(1,sizeof(student));
		stu->id = i + 1;
		stu->name = stu_name[i];

		student_add(&student_list,stu);
	}

	//��ӡ���нڵ�
	student_foreach(&student_list,student_print,(void *)0);
    
    printf("==================\n\n");
	
	//ɾ���ڵ�
	student_del(&student_list,1);
	student_foreach(&student_list,student_print,(void *)0);
    printf("==================\n\n");

	//ɾ���ڵ�
	student_del(&student_list,5);
	student_foreach(&student_list,student_print,(void *)0);
    printf("==================\n\n");
	
	return 0;
}
