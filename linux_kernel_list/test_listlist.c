#include<stdio.h>
#include<stdlib.h>
#include "../app/listlist.h"

#define STUDENT_FREE_MEMORY

//声明链表节点
typedef struct {
	int id;
	char *name;
	struct listnode _list;
}student;

//遍历函数指针
typedef void (*student_foreach_fun)(student *stu,void *data);


//声明链表
static list_declare(student_list);

//添加节点
int student_add(struct listnode *list,student *stu)
{
	list_init(&stu->_list);
	list_add_tail(list,&stu->_list);	
}

//删除节点，释放节点空间
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
			//释放节点空间
			free(stu);
			stu = NULL;
#endif
			return 1;
			
		}
		
	}

	return 0;
}

//节点遍历
void student_foreach(struct listnode *list,student_foreach_fun fun,void *data)
{
	struct listnode *node;
	student *stu;
	list_for_each(node,list){
		stu = node_to_item(node,student,_list);
		fun(stu,data);
	}

}

//打印节点信息
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
	//添加节点
	for(i=0;i<len;i++){
		stu = calloc(1,sizeof(student));
		stu->id = i + 1;
		stu->name = stu_name[i];

		student_add(&student_list,stu);
	}

	//打印所有节点
	student_foreach(&student_list,student_print,(void *)0);
    
    printf("==================\n\n");
	
	//删除节点
	student_del(&student_list,1);
	student_foreach(&student_list,student_print,(void *)0);
    printf("==================\n\n");

	//删除节点
	student_del(&student_list,5);
	student_foreach(&student_list,student_print,(void *)0);
    printf("==================\n\n");
	
	return 0;
}
