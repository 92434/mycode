#include "pcie.h"

#define SPEED_MB (1024 * 1024)

//static struct timeval start_time = {0};
long unsigned int get_op_tx_count(pcie_dma_t *dma);
long unsigned int get_op_rx_count(pcie_dma_t *dma);
static void test_performance(kc705_pci_dev_t *kc705_pci_dev)
{
	int i;
	long unsigned int total_tx_speed = 0;
	long unsigned int total_rx_speed = 0;
	//struct timeval stop_time;

	//do_gettimeofday(&stop_time);

	//mydebug("stop_time.tv_sec:%lu\n", stop_time.tv_sec);
	//mydebug("start_time.tv_sec:%lu\n", start_time.tv_sec);
	//mydebug("stop_time.tv_usec:%lu\n", stop_time.tv_usec);
	//mydebug("start_time.tv_usec:%lu\n", start_time.tv_usec);
	//
	if(kc705_pci_dev->dma_count == 0) {
		return;
	}

	for(i = 0; i < kc705_pci_dev->dma_count; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		long unsigned int tx_speed = get_op_tx_count(dma);
		long unsigned int rx_speed = get_op_rx_count(dma);

		if(rx_speed + tx_speed > 0) {
			printk("[%02d]%-*.*s(MB/s)|U:%4lu.%06lu|D:%4lu.%06lu|A:%4lu.%06lu\n",
				   i,
				   20,
				   20,
				   dma->devname,
				   tx_speed / SPEED_MB, (1000 * 1000) * (tx_speed % SPEED_MB) / SPEED_MB,
				   rx_speed / SPEED_MB, (1000 * 1000) * (rx_speed % SPEED_MB) / SPEED_MB,
				   (rx_speed + tx_speed) / SPEED_MB, (1000 * 1000) * ((rx_speed + tx_speed) % SPEED_MB) / SPEED_MB);
		}

		total_tx_speed += tx_speed;
		total_rx_speed += rx_speed;

	}

	if(total_tx_speed + total_rx_speed > 0) {
		printk("%-*.*s(MB/s)|U:%4lu.%06lu|D:%4lu.%06lu|A:%4lu.%06lu\n",
			   24,
			   20,
			   "all",
			   total_tx_speed / SPEED_MB, (1000 * 1000) * (total_tx_speed % SPEED_MB) / SPEED_MB,
			   total_rx_speed / SPEED_MB, (1000 * 1000) * (total_rx_speed % SPEED_MB) / SPEED_MB,
			   (total_tx_speed + total_rx_speed) / SPEED_MB, (1000 * 1000) * ((total_tx_speed + total_rx_speed) % SPEED_MB) / SPEED_MB);
		printk("\n");
	}

	//do_gettimeofday(&start_time);
}

//static DEFINE_MUTEX(work_lock);
//mutex_lock(&work_lock);
//mutex_unlock(&work_lock);

static void work_func(struct work_struct *work)
{
	kc705_pci_dev_t *kc705_pci_dev = container_of(work, kc705_pci_dev_t, timer_work);

	test_performance(kc705_pci_dev);
}

static void timer_func(unsigned long __opaque)
{
	timer_data_t *ptimer_data = (timer_data_t *)__opaque;
	unsigned long tmo = msecs_to_jiffies(ptimer_data->ms);
	struct timer_list *tl = ptimer_data->tl;

	schedule_work(ptimer_data->timer_work);
	tl->expires = jiffies + tmo;
	add_timer(tl);
}

void start_statistic_timer(kc705_pci_dev_t *kc705_pci_dev)
{
	if(kc705_pci_dev->ptimer_data == NULL) {
		INIT_WORK(&(kc705_pci_dev->timer_work), work_func);
		kc705_pci_dev->ptimer_data = alloc_timer(1000, timer_func, &(kc705_pci_dev->timer_work));
	}
}

void stop_statistic_timer(kc705_pci_dev_t *kc705_pci_dev)
{
	cancel_work_sync(&(kc705_pci_dev->timer_work));

	if(kc705_pci_dev->ptimer_data != NULL) {
		free_timer(kc705_pci_dev->ptimer_data);
		kc705_pci_dev->ptimer_data = NULL;
	}
}

void inc_dma_op_tx_count(pcie_dma_t *dma, long unsigned int count)
{
	dma->tx_count += count;
}

void inc_dma_op_rx_count(pcie_dma_t *dma, long unsigned int count)
{
	dma->rx_count += count;
}

long unsigned int get_op_tx_count(pcie_dma_t *dma)
{
	long unsigned int count = dma->tx_count;
	dma->tx_count = 0;
	return count;
}

long unsigned int get_op_rx_count(pcie_dma_t *dma)
{
	long unsigned int count = dma->rx_count;
	dma->rx_count = 0;
	return count;
}
