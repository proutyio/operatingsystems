/*
 * elevator noop
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

/*
 * Variable to keep track of diskhead position
 */
unsigned long long disk_pos;

struct noop_data {
  struct list_head queue;
};

static void noop_merged_requests(struct request_queue *q, struct request *rq,
				 struct request *next)
{
  list_del_init(&next->queuelist);
}

static int noop_dispatch(struct request_queue *q, int force)
{
  struct noop_data *nd = q->elevator->elevator_data;
  char *op;

  if (!list_empty(&nd->queue)) {
    struct request *rq;
    /* Grab next request ahead of list head */
    rq = list_entry(nd->queue.next, struct request, queuelist);

    if (rq_data_dir(rq))
      op = "Write";
    else
      op = "Read";

    printk("\n%s%llu\n%s%s\n", "In Dispatch\nRequest: ",
	   (unsigned long long)blk_rq_pos(rq), "Operation: ", op);

    list_del_init(&rq->queuelist);
    elv_dispatch_sort(q, rq);
    disk_pos = blk_rq_pos(rq);

    printk("\n%s%llu\n", "Disk position: ", disk_pos);
    
    return 1;
  }
  return 0;
}

static void noop_add_request(struct request_queue *q, struct request *rq)
{
  struct noop_data *nd = q->elevator->elevator_data;
  struct list_head *pos = NULL;
  struct request *next_req, *cur_req, *prev_req;

  sector_t cur_sector, rq_sector, next_sector; 
  char *op;

  if (rq_data_dir(rq))
    op = "Write";
  else
    op = "Read";

  printk("\n%s%llu\n%s%s\n", "In Add\nRequest block pos: ",
  	 (unsigned long long)blk_rq_pos(rq), "Operation: ", op);

  /*
   * Check for empty list, if true then just insert, else check neighboor values
   */

  //  if(list_empty(&nd->queue)){
  //  list_add(&rq->queuelist, &nd->queue);
  //} else {

  // printk("\n%s\n", "Else clause");
  list_for_each(pos, &nd->queue){
  /*
   * Check for larger sector request, when request sector is less than the current
   * link, then break and insert before that link
   */
      next_req = list_entry(pos->next, struct request, queuelist);
      prev_req = list_entry(pos->prev, struct request, queuelist);
      cur_req = list_entry(pos, struct request, queuelist);
    
      cur_sector = blk_rq_pos(cur_req);
      rq_sector = blk_rq_pos(rq);
      next_sector =  blk_rq_pos(next_req);

      printk("\n%s\n%llu%s%llu%s%s\n", "cur_sector > rq_sector", (unsigned long long)cur_sector,
	     "  >  ", (unsigned long long)rq_sector, " = ", ((cur_sector > rq_sector) ? "TRUE" : "FALSE"));
     
      if(cur_sector > rq_sector){

	printk("\n%s\n%s%llu\n%s%llu\n%s%llu\n", "In for loop (BREAK):", "Next sector: ",
	       (unsigned long long)blk_rq_pos(next_req),
	       "Current sector: ", (unsigned long long)blk_rq_pos(cur_req),
	       "Prev sector: ", (unsigned long long)blk_rq_pos(prev_req));
	
	break;
      }
    }
  //} 
    
    cur_req = list_entry(&nd->queue, struct request, queuelist);
    next_req = list_entry(nd->queue.next, struct request, queuelist);
    prev_req = list_entry(nd->queue.prev, struct request, queuelist);
  
    // printk("\n%s\n%s%llu\n%s%llu\n%s%llu\n", "Out of for loop:", "Next request sector: ",
    //	   (unsigned long long)blk_rq_pos(next_pos),
    //	   "Current request sector: ", (unsigned long long)blk_rq_pos(cur_pos),
    //	   "Previous request sector: ", (unsigned long long)blk_rq_pos(prev_pos));
    
    // printk("\n%s%llu\n", "Current request sector: ", (unsigned long long)rq_end_sector(cur_pos));

    /* Adding to tail will ensure no requests are put infront of the head position */
    list_add_tail(&rq->queuelist, pos);
}

static struct request *
noop_former_request(struct request_queue *q, struct request *rq)
{
  struct noop_data *nd = q->elevator->elevator_data;

  if (rq->queuelist.prev == &nd->queue)
    return NULL;
  return list_entry(rq->queuelist.prev, struct request, queuelist);
}

static struct request *
noop_latter_request(struct request_queue *q, struct request *rq)
{
  struct noop_data *nd = q->elevator->elevator_data;

  if (rq->queuelist.next == &nd->queue)
    return NULL;
  return list_entry(rq->queuelist.next, struct request, queuelist);
}

static int noop_init_queue(struct request_queue *q, struct elevator_type *e)
{
  struct noop_data *nd;
  struct elevator_queue *eq;

  printk("\n%s\n", "Initialized Noop Scheduler");

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

static void noop_exit_queue(struct elevator_queue *e)
{
  struct noop_data *nd = e->elevator_data;

  BUG_ON(!list_empty(&nd->queue));
  kfree(nd);
}

static struct elevator_type elevator_noop = {
  .ops = {
    .elevator_merge_req_fn= noop_merged_requests,
    .elevator_dispatch_fn= noop_dispatch,
    .elevator_add_req_fn= noop_add_request,
    .elevator_former_req_fn= noop_former_request,
    .elevator_latter_req_fn= noop_latter_request,
    .elevator_init_fn= noop_init_queue,
    .elevator_exit_fn= noop_exit_queue,
  },
  .elevator_name = "noop",
  .elevator_owner = THIS_MODULE,
};

static int __init noop_init(void)
{
  return elv_register(&elevator_noop);
}

static void __exit noop_exit(void)
{
  elv_unregister(&elevator_noop);
}

module_init(noop_init);
module_exit(noop_exit);


MODULE_AUTHOR("Jens Axboe");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("No-op IO scheduler");
