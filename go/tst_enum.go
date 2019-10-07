package main

import (
	"fmt"
	"log"
)

type BuildStage int

const (
	DictStatic       = 0
	DictParasIniting = 1
	DictHdsfPulling  = 2
	DictBackuping    = 3
	DictWriting      = 4
	DictSuccess      = 5
	DictFailed       = 6

	BuildUnstart BuildStage = iota + 3
	BuildPrepare
	BuildInitdb
	BuildFetchData // pull & uncompress dict data
	BuildInserting
	BuildCompacting
	BuildComplete
)

func (b BuildStage) String() string {
	switch b {
	case BuildUnstart:
		return "BuildUnstart"
	case BuildPrepare:
		return "BuildPrepare"
	case BuildInitdb:
		return "BuildInitdb"
	case BuildFetchData:
		return "BuildFetchData"
	case BuildInserting:
		return "BuildInserting"
	case BuildCompacting:
		return "BuildCompacting"
	case BuildComplete:
		return "BuildComplete"
	default:
		return "Unknown"
	}
}

type SyncStage int

//trp进行rsync时的check交互状态
const (
	TrpRsyncStageInit      = 0
	TrpRsyncStageDeploying = 1
	TrpRsyncStageSuccess   = 2
	TrpRsyncStageFailed    = 3
	//TrpRsyncStageDrop	  = 4 //因buildmode改变，放弃sync，

	DeployUnstart SyncStage = iota + 6
	DeploySyncing
	DeployComplete
)

func (s SyncStage) String() string {
	switch s {
	case DeployUnstart:
		return "DeployUnstart"
	case DeploySyncing:
		return "DeploySyncing"
	case DeployComplete:
		return "DeployComplete"
	default:
		return "Unknown"
	}
}

func main() {
	fmt.Println(DeployUnstart)
	fmt.Printf("%d\n", DeployUnstart)
	fmt.Println(DeploySyncing)
	fmt.Printf("%d\n", DeploySyncing)

	fmt.Println(BuildUnstart)
	fmt.Printf("%d\n", BuildUnstart)

	req := 0
	res := 1
	trace := 2
	stage := 10
DONE:
	for {
		switch BuildStage(stage) {
		case BuildUnstart:
			log.Printf("[checkBuildStage] stage:%v req:%v res:%v trace:%v, dict build failed",
				BuildStage(stage), req, res, trace)
			break DONE
		case BuildPrepare:
		case BuildInitdb:
		case BuildFetchData:
		case BuildInserting:
		case BuildCompacting:
			log.Printf("[checkBuildStage] stage:%v req:%v res:%v trace:%v, dict build failed",
				BuildStage(stage), req, res, trace)
			break DONE
		case BuildComplete:
			log.Printf("[checkBuildStage] stage:%v req:%v res:%v trace:%v, dict build failed",
				BuildStage(stage), req, res, trace)
			break DONE
		default:
			log.Printf("[checkBuildStage] stage:%v req:%v res:%v trace:%v, dict build failed",
				BuildStage(stage), req, res, trace)
			break DONE
		}
	}
}
