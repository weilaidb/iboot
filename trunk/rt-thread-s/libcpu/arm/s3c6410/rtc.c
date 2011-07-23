/*
 * File      : rtc.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-04-26     yi.qiu       	first version
 * 2010-03-18     Gary Lee	add functions such as GregorianDay
 *                             		and rtc_time_to_tm
 * 2009-03-20     yi.qiu       	clean up
 */

#include <rtthread.h>
#include <time.h>
#include <s3c6410.h>

// #define RTC_DEBUG
#define BCD2BIN(n)		(((((n) >> 4) & 0x0F) * 10) + ((n) & 0x0F))
#define BIN2BCD(n)		((((n) / 10) << 4) | ((n) % 10))

static inline void RTC_ENABLE(void)
{
	s3c_writel(s3c_readl(RTCCON) | 0x01, RTCCON);	/*RTC read and write enable */
}
static inline void RTC_DISABLE(void)
{
	s3c_writel(s3c_readl(RTCCON) & ~0x01, RTCCON);	/* RTC read and write disable */
}

/**
 * This function get rtc time
 */
void rt_hw_rtc_get(struct tm *ti)
{
	rt_uint8_t sec, min, hour, mday, wday, mon, year;

	/* enable access to RTC registers */
	RTC_ENABLE();

	/* read RTC registers */
	do
	{
		sec 	= s3c_readl(BCDSEC);
		min 	= s3c_readl(BCDMIN);
		hour 	= s3c_readl(BCDHOUR);
		mday	= s3c_readl(BCDDATE);
		wday 	= s3c_readl(BCDDAY);
		mon 	= s3c_readl(BCDMON);
		year 	= s3c_readl(BCDYEAR);
    } while (sec != s3c_readl(BCDSEC));

#ifdef RTC_DEBUG
	rt_kprintf("sec:%x min:%x hour:%x mday:%x wday:%x mon:%x year:%x\n",
		sec, min, hour, mday, wday, mon, year);
#endif

	/* disable access to RTC registers */
	RTC_DISABLE();

	ti->tm_sec  	= BCD2BIN(sec  & 0x7F);
	ti->tm_min  	= BCD2BIN(min  & 0x7F);
	ti->tm_hour 	= BCD2BIN(hour & 0x3F);
	ti->tm_mday 	= BCD2BIN(mday & 0x3F);
	ti->tm_mon  	= BCD2BIN(mon  & 0x1F);
	ti->tm_year 	= BCD2BIN(year);
	ti->tm_wday 	= BCD2BIN(wday & 0x07);
	ti->tm_yday 	= 0;
	ti->tm_isdst 	= 0;
}

/**
 * This function set rtc time
 */
void rt_hw_rtc_set(struct tm *ti)
{
	rt_uint8_t sec, min, hour, mday, wday, mon, year;

	year	= BIN2BCD(ti->tm_year);
	mon 	= BIN2BCD(ti->tm_mon);
	wday 	= BIN2BCD(ti->tm_wday);
	mday 	= BIN2BCD(ti->tm_mday);
	hour 	= BIN2BCD(ti->tm_hour);
	min 	= BIN2BCD(ti->tm_min);
	sec 	= BIN2BCD(ti->tm_sec);

	/* enable access to RTC registers */
	RTC_ENABLE();

	do{
		/* write RTC registers */
		s3c_writel(sec, BCDSEC);
		s3c_writel(min, BCDMIN);
		s3c_writel(hour, BCDHOUR);
		s3c_writel(mday, BCDDATE);
		s3c_writel(wday, BCDDAY);
		s3c_writel(mon, BCDMON);
		s3c_writel(year, BCDYEAR);
	}while (sec != s3c_readl(BCDSEC));
	
	/* disable access to RTC registers */
	RTC_DISABLE();
}

/**
 * This function reset rtc
 */
void rt_hw_rtc_reset (void)
{
	register rt_uint32_t regv;

	regv = s3c_readl(RTCCON);
	regv &= ~0x06;
	regv |= 0x08;
	s3c_writel(regv, RTCCON);

	regv = s3c_readl(RTCCON);
	regv &= ~(0x08|0x01);
	s3c_writel(regv, RTCCON);
}

static struct rt_device rtc;
static rt_err_t rtc_open(rt_device_t dev, rt_uint16_t oflag)
{
	RTC_ENABLE();
	return RT_EOK;
}

static rt_err_t rtc_close(rt_device_t dev)
{
	RTC_DISABLE();
	return RT_EOK;
}

static rt_size_t rtc_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	return RT_EOK;
}

static rt_err_t rtc_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
	struct tm tm, *tm_ptr;
    time_t *time;
	RT_ASSERT(dev != RT_NULL);

	time = (time_t *)args;
	switch (cmd)
	{
	case RT_DEVICE_CTRL_RTC_GET_TIME:
		/* read device */
		rt_hw_rtc_get(&tm);
		*((rt_time_t *)args) = mktime(&tm);
		break;

	case RT_DEVICE_CTRL_RTC_SET_TIME:
		tm_ptr = localtime(time);
		/* write device */
		rt_hw_rtc_set(tm_ptr);
		break;
	}

	return RT_EOK;
}

void rt_hw_rtc_init(void)
{
	rtc.type	= RT_Device_Class_RTC;

	/* register rtc device */
	rtc.init 	= RT_NULL;
	rtc.open 	= rtc_open;
	rtc.close	= rtc_close;
	rtc.read 	= rtc_read;
	rtc.write	= RT_NULL;
	rtc.control = rtc_control;
	
	/* no private */
	rtc.user_data = RT_NULL;
	
	rt_device_register(&rtc, "rtc", RT_DEVICE_FLAG_RDWR);
}

#include <finsh.h>
void list_date()
{
	time_t time;
	rt_device_t device;

	device = rt_device_find("rtc");
	if (device != RT_NULL)
	{
		rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, &time);

		rt_kprintf("%d, %s\n", time, ctime(&time));
	}
}
FINSH_FUNCTION_EXPORT(list_date, list date);
