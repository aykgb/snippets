package main

import (
	"fmt"
	"sync"
	"time"
)

type RequestLimiter struct {
	QPS             int64      `json:"qps"`
	AllowedRequests int64      `json:"allowed_requests"`
	LastUpdatedTime time.Time  `json:"last_updated_time"`
	mtx             sync.Mutex //! 多协程访问
}

func NewRequestLimiter(qps int64) *RequestLimiter {
	return &RequestLimiter{
		QPS:             qps,
		AllowedRequests: 1, //! 初始化为1
		LastUpdatedTime: time.Now(),
	}
}

func (r *RequestLimiter) Allowed() (allowed bool) {
	r.mtx.Lock()
	defer r.mtx.Unlock()

	now := time.Now() //! 这里暂存当前时间可能作为更新时间戳
	duration := now.Sub(r.LastUpdatedTime)
	delta := float64(r.QPS) / float64(time.Second) * float64(duration)
	if delta < 1 && r.AllowedRequests < 1 {
		return false
	}
	if delta < 1 && r.AllowedRequests > 1 {
		r.AllowedRequests-- //! 返回true，则AllowedRequests需要减1
		return true
	}
	// delta > 1
	r.LastUpdatedTime = now
	r.AllowedRequests += int64(delta)
	if r.AllowedRequests > r.QPS { //! 当超过qps需要重置AllowedRequests
		r.AllowedRequests = r.QPS
	}
	r.AllowedRequests-- //! 返回true，则AllowedRequests需要减1

	return true
}

func RequestLimiterTest(limitQPS, reqQPS, sec int64) (counter int64) {
	fmt.Printf("limit qps %d, request qps %d\n", limitQPS, reqQPS)
	beg := time.Now()
	limiter := NewRequestLimiter(limitQPS)
	counter = 1
	for {
		if limiter.Allowed() {
			fmt.Printf("==> qps %d duration:%3.3f seconds %dst request allowed. \n",
				limiter.QPS,
				time.Now().Sub(beg).Seconds(),
				counter)
			counter++
		}
		time.Sleep(time.Second / time.Duration(reqQPS+1))
		if time.Now().Sub(beg) > time.Duration(int64(time.Second)*sec) {
			break
		}
	}

	return counter
}
