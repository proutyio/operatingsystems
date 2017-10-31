/*
 * Elevator clook: based on native noop scheduler
 * 
 * Written by Nathaniel Whitlock and Kyle Prouty
 * Group 18
 * 
 * Changes were made to the following functions:
 * - clook_dispatch()
 * - clook_add_request()
 * - clook_init_queue()
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

unsigned long long disk_pos;

struct clook_data {
	struct list_head queue;
};

static void clook_merged_requests(struct request_queue *q, struct request *rq,
				 struct request *next)
{
	list_del_init(&next->queuelist);
}

/*
 * Added print statements to monitor the dispatch of requests
 * as well as update for global disk_pos variable
 */
static int clook_dispatch(struct request_queue *q, int force)
{
	struct clook_data *nd = q->elevator->elevator_data;

	char *op;

	if (!list_empty(&nd->queue)) {
	  struct request *rq;
	  rq = list_entry(nd->queue.next, struct request, queuelist);
	  
	  if (rq_data_dir(rq))
	    op = "Write";
	  else
	    op = "Read";
	  
	  printk("\n%s%s %llu\n", "[CLOOK] dsp ", op, (unsigned long long)blk_rq_pos(rq)); 

	  list_del_init(&rq->queuelist);
	  elv_dispatch_sort(q, rq);
	  disk_pos = blk_rq_pos(rq);
	    
	  return 1;
	}
	return 0;
}

/*
 * Made the following changes to this function:
 * - Print statements to monitor incoming requests
 * - Loop to iterated through queue nodes
 * - Conditional logic to force sorted insert
 */
static void clook_add_request(struct request_queue *q, struct request *rq)
{
	struct clook_data *nd = q->elevator->elevator_data;
	struct list_head *pos = NULL;
	struct request *cur_req;

	sector_t cur_sector, rq_sector;
	char *op;
	
	/*
	* Check for read or write operation
	*/
        if (rq_data_dir(rq))
          op = "Write";
        else
          op = "Read";

        /*
         * Print status message
         */
        //printk("\n%s%s%s%llu\n", "[CLOOK] add ", op, " ", (unsigned long long)blk_rq_pos(rq));

	/*
	 * Iterate through each node of the queue, updating pos on each iteration
 	 */
	list_for_each(pos, &nd->queue){
	  
	  /*
	   * Access curret queue request and translate queue request and incoming
	   * request to sector values
	   */
	  cur_req = list_entry(pos, struct request, queuelist);
	  rq_sector = blk_rq_pos(rq);
	  cur_sector = blk_rq_pos(cur_req); 

	  /*
	   * Check to see if the current sector in the queue is larger than the request
	   * sector and less than the disk head pos. If this is true, break and insert on the tail side
	   */
	  if(cur_sector > rq_sector){
	    break;
	  }
	  
	}
	
	/*
	 * Always add to tail side of node
	 */
	list_add_tail(&rq->queuelist, pos);

	/*
	 * Iterate through and print out queue, somewhat used for testing
	 */
	printk("\n%s\n%s\n", "Queue Contents:", "---------------");
	list_for_each(pos,&nd->queue){
	  printk("%llu\n",(unsigned long long)blk_rq_pos(list_entry(pos, struct request, queuelist)));
	}
	printk("\n%s -- %llu\n%s\n", "Last head position ", disk_pos, "---------------");
}
static struct request *
clook_former_request(struct request_queue *q, struct request *rq)
{
	struct clook_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.prev == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.prev, struct request, queuelist);
}

static struct request *
clook_latter_request(struct request_queue *q, struct request *rq)
{
	struct clook_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.next == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.next, struct request, queuelist);
}

/*
 * Added print statement once the scheduler is initialized 
 */
static int clook_init_queue(struct request_queue *q, struct elevator_type *e)
{
	struct clook_data *nd;
	struct elevator_queue *eq;

	printk("\n%s\n\n" , "CLOOK Scheduler Initialized...");
 
	eq = elevator_alloc(q, e);
	if (!eq)
		return -ENOMEM;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd) {
		kobject_put(&eq->kobj);
		return -ENOMEM;
	}
	eq->elevator_data = nd;

	INIT_LIST_HEAD(&nd->queue);

	spin_lock_irq(q->queue_lock);
	q->elevator = eq;
	spin_unlock_irq(q->queue_lock);
	return 0;
}

static void clook_exit_queue(struct elevator_queue *e)
{
	struct clook_data *nd = e->elevator_data;

	BUG_ON(!list_empty(&nd->queue));
	kfree(nd);
}

static struct elevator_type elevator_clook = {
	.ops = {
		.elevator_merge_req_fn		= clook_merged_requests,
		.elevator_dispatch_fn		= clook_dispatch,
		.elevator_add_req_fn		= clook_add_request,
		.elevator_former_req_fn		= clook_former_request,
		.elevator_latter_req_fn		= clook_latter_request,
		.elevator_init_fn		= clook_init_queue,
		.elevator_exit_fn		= clook_exit_queue,
	},
	.elevator_name = "clook",
	.elevator_owner = THIS_MODULE,
};

static int __init clook_init(void)
{
	return elv_register(&elevator_clook);
}

static void __exit clook_exit(void)
{
	elv_unregister(&elevator_clook);
}

module_init(clook_init);
module_exit(clook_exit);


MODULE_AUTHOR("Nathaniel Whitlock & Kyle Prouty");
MODULE_LICENSE("PNW");
MODULE_DESCRIPTION("CLOOK IO scheduler");
