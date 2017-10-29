/*
 * Elevator CLOOK
 * 
 * Written by Nathaniel Whitlock and Kyle Prouty
 * Group 18 -- CS 444
 *
 * Changes were made to the following functions:
 * - clook_dispatch()
 * - clook _add_request()
 * - clook_init_queue
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

struct clook_data {
  struct list_head queue;
};

static void clook_merged_requests(struct request_queue *q, struct request *rq,
				  struct request *next)
{
  list_del_init(&next->queuelist);
}

/*
 * Added print statement to monitor request dispatch
 */
static int clook_dispatch(struct request_queue *q, int force)
{
  struct clook_data *cd = q->elevator->elevator_data;
  char *op;

  if (!list_empty(&cd->queue)) {
    struct request *rq;
    rq = list_entry(nd->queue.next, struct request, queuelist);
    
    if (rq_data_dir(rq))
      op = "Write";
    else
      op = "Read";

    printk("\n%s%s %llu\n", "[CLOOK] dsp ", op, (unsigned long long)blk_rq_pos(rq));

    list_del_init(&rq->queuelist);
    elv_dispatch_sort(q, rq);
    return 1;
  }
  return 0;
}

/*
 * Following changes were made:
 * - Added print statement to monitor request addition
 * - Controlled insert to order requests in reguards to disk sector  
 */
static void clook_add_request(struct request_queue *q, struct request *rq)
{
  struct clook_data *cd = q->elevator->elevator_data;
  struct list_head *pos = NULL;
  struct request *next_req, *cur_req, *prev_req;

  sector_t cur_sector, rq_sector, next_sector;
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
  printk("\n%s%s%s%llu\n", "[CLOOK] add ", op, " ", (unsigned long long)blk_rq_pos(rq));
  list_for_each(pos, &nd->queue){
    next_req = list_entry(pos->next, struct request, queuelist);
    prev_req = list_entry(pos->prev, struct request, queuelist);
    cur_req = list_entry(pos, struct request, queuelist);

    next_sector = blk_rq_pos(next_req);
    rq_sector = blk_rq_pos(rq);
    cur_sector = blk_rq_pos(cur_req);

    printk("\n%s\n%llu%s%llu%s%s\n", "cur_sector > rq_sector", (unsigned long long)cur_sector,
	   "  >  ", (unsigned long long)rq_sector, " = ", ((cur_sector > rq_sector) ? "TRUE" : "FALSE"));

    /*
     * Check to see if the current sector in the queue is larger than the request
     * sector, if this is true break and insert on the tail side. This keeps the largest
     * request sector before the head position so that the head can proceed through the
     * circular doubly linked list to smaller sectors once the large have been processed,
     * thus created a circular motion
     */
    if(cur_sector > rq_sector){
      printk("\n%s\n%s%llu\n%s%llu\n%s%llu\n", "In for loop (BREAK):", "Next sector: ",
	     (unsigned long long)blk_rq_pos(next_req),
	     "Current sector: ", (unsigned long long)blk_rq_pos(cur_req),
	     "Prev sector: ", (unsigned long long)blk_rq_pos(prev_req));

      break;
    }
  }

  list_add_tail(&rq->queuelist, &cd->queue);
}

static struct request *
clook_former_request(struct request_queue *q, struct request *rq)
{
  struct clook_data *cd = q->elevator->elevator_data;

  if (rq->queuelist.prev == &cd->queue)
    return NULL;
  return list_entry(rq->queuelist.prev, struct request, queuelist);
}

static struct request *
clook_latter_request(struct request_queue *q, struct request *rq)
{
  struct clook_data *cd = q->elevator->elevator_data;

  if (rq->queuelist.next == &cd->queue)
    return NULL;
  return list_entry(rq->queuelist.next, struct request, queuelist);
}

/*
 * Added print statement to mark scheduler initialization
 */
static int clook_init_queue(struct request_queue *q, struct elevator_type *e)
{
  struct clook_data *cd;
  struct elevator_queue *eq;

  eq = elevator_alloc(q, e);
  if (!eq)
    return -ENOMEM;

  nd = kmalloc_node(sizeof(*cd), GFP_KERNEL, q->node);
  if (!nd) {
    kobject_put(&eq->kobj);
    return -ENOMEM;
  }
  eq->elevator_data = nd;

  INIT_LIST_HEAD(&cd->queue);

  spin_lock_irq(q->queue_lock);
  q->elevator = eq;
  spin_unlock_irq(q->queue_lock);
  return 0;
}

static void clook_exit_queue(struct elevator_queue *e)
{
  struct clook_data *cd = e->elevator_data;

  BUG_ON(!list_empty(&cd->queue));
  kfree(nd);
}

static struct elevator_type elevator_clook = {
  .ops = {
    .elevator_merge_req_fn= clook_merged_requests,
    .elevator_dispatch_fn= clook_dispatch,
    .elevator_add_req_fn= clook_add_request,
    .elevator_former_req_fn= clook_former_request,
    .elevator_latter_req_fn= clook_latter_request,
    .elevator_init_fn= clook_init_queue,
    .elevator_exit_fn= clook_exit_queue,
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
