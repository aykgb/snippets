/***********************************BEGIN***************************************
 **                      CONFIDENTIAL AND PROPRIETARY
 **         Copyright (C) 2005-2012 Tellabs, All Rights Reserved
 *******************************************************************************
 *
 * File:    PSM_HAL_oam.cpp
 * Type:    C++ source
 * Description:
 *      Class implementation of PSM OAM HAL server partition.
 *
 *
 ***********************************END*****************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "PSM_HAL_oam.h"
#include "Tlab_utils.h"
#include "Tlab_utils_profile.h"
#include "PSM_HAL_utils.h"
#include <exception>
#include <stdexcept>

extern PSM_HAL_OAM *oam;
extern int oam_wb_print_flag;

extern int gOPE_db_lock_unlock_debug_flag;

//-----------------------------------------------------------------------------
//       Class:  FPAL_SOAM_protocol_stub
//
//      Method:  timer_action
//
// Description:
//-----------------------------------------------------------------------------
void PSM_HAL_OAM::timer_action(void)
{
    oam->timer_expiry_handler();
}

//-----------------------------------------------------------------------------
//       Class:  FPAL_SOAM_protocol_stub
//
//      Method:  timer_expiry_handler
//
// Description:
//-----------------------------------------------------------------------------
HAL_ERR PSM_HAL_OAM::timer_expiry_handler(void)
{
    if (!isShutdownWarm) {
        poke_pm_cont_in_fpga_ctrl();
    }

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  PSM_HAL_OAM                                            **/
/**                                                                    **/
/**  Description:                                                      **/
/**      Constructor for PSM_HAL_OAM class.                            **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      equip_max - number of pieces of equipment we are tracking.    **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      none -                                                        **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**********************************END***********************************/
PSM_HAL_OAM::PSM_HAL_OAM(unsigned int equip_max) : HAL_OAM(equip_max), isShutdownWarm(false), isBootWarm(false)
{
    //initiate DB
    PSM_HAL_OAM_ope_db_api::instance();

    memset(&global, 0x00, sizeof(struct s_HAL_SOAM_global));

    //setup timer
    {
        callback_data = new Tlab_utils_timer_mgr_callback_data();

        callback_data->action =
            reinterpret_cast<Tlab_utils_timer_mgr_action>
            (&PSM_HAL_OAM::timer_action);

        Tlab_utils_timer_cfg_data cfg_data;

        cfg_data.callback_data = callback_data;
        cfg_data.period = 0;
        cfg_data.type = e_timer_periodic;
        cfg_data.priority = e_timer_high;

        if(Tlab_utils_timer_mgr_wait::Instance()->create(
                    &timer_id, &cfg_data) != TLAB_SUCCESS) {
            throw(-1);
        }
    }

#ifdef __TARGET__
    arad_unit_id = 0;
    arad_slot_id = 0;
    aradSdk = NULL;
#endif //__TARGET__

    send_fake_event = 0;
    trc = NULL;
    expert_trc = NULL;
    b_arad_oamp_enabled = false;
    b_fpga_oamp_enabled = true;
    b_fpga_dummy_meg_id_enabled = false;
    parser_dump_db = NULL;
    parser_me = NULL;
    parser_mep = NULL;
    parser_mip = NULL;
    parser_rmep = NULL;
    parser_fm_client = NULL;
    parser_tx_frame = NULL;
    fpga_driver = NULL;
    fpga_dma_driver = NULL;
    cli_menu = NULL;
    cli_menu_expert = NULL;
    g_oam_cmd_tbl_size = 0;
    g_expert_oam_cmd_tbl_size = 0;
    lbrIndexMask = 0;
    memset(&state2544, 0,sizeof(state2544));
    memset(&oam_ev_data, 0, sizeof(HAL_OAM_EVENT_DATA));
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  ~PSM_HAL_OAM                                           **/
/**                                                                    **/
/**  Description:                                                      **/
/**      Destructor for PSM_HAL_OAM class.                             **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      none -                                                        **/
/**                                                                    **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      none -                                                        **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**********************************END***********************************/
PSM_HAL_OAM::~PSM_HAL_OAM(void)
{
    deinit_oam_debug();

#ifdef __TARGET__
    //detach OAM ARAD
#if defined WARM_RESET_WORK
    if (!isShutdownWarm)
#endif
    {
        deinit_oam_arad();
    }
#endif //__TARGET__

    //detach OAM FPGA
    deinit_oam_fpga();

    Tlab_utils_timer_mgr_wait::Instance()->remove(timer_id);
    delete callback_data;

    delete PSM_HAL_OAM_OPE_DB_API_INST;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  InitCold                                               **/
/**                                                                    **/
/**  Description:                                                      **/
/**     Init function for cold boot                                    **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      None.                                                         **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**********************************END***********************************/
void PSM_HAL_OAM::InitCold(void)
{

    unsigned long skipInit = 0;
    isBootWarm = false;
    char *skipInitString;

    HAL_INFO_PRINT("PSM_HAL_OAM: InitCold() executing");

    /*
     * For DEBUG purposes, don't run the init code if environment
     *  variable HalServerSkipOamInit is set to non-zero
     */
    skipInitString = getenv("HalServerSkipOamInit");
    if (skipInitString !=  NULL) {
        skipInit = strtoul(skipInitString, NULL, 0);
    }

    if ( skipInit ) {
        HAL_INFO_PRINT("***************************************************************\n");
        HAL_INFO_PRINT("*** Environment variable \"HalServerSkipOamInit\" is set!\n");
        HAL_INFO_PRINT("*** Skipping initialization of OAM partition\n");
        HAL_INFO_PRINT("***************************************************************\n");
    } else {
        //initialize OAM portion of the FPGA
        if(init_oam_fpga() != HAL_SUCCESS) {
            throw(HAL_ERROR);
        }

#ifdef __TARGET__
        if(init_oam_arad() != HAL_SUCCESS) {
            throw(HAL_ERROR);
        }
#endif //__TARGET__

        if(init_oam_debug() != HAL_SUCCESS) {
            throw(HAL_ERROR);
        }

        global.pm_cont_interval =
            HAL_SOAM_GLOBAL_DEF_PM_CONT_INTERVAL_MSEC;

        Tlab_utils_timer_cfg_data cfg_data;
        cfg_data.period = global.pm_cont_interval;
        Tlab_utils_timer_mgr_wait::Instance()->start(timer_id, &cfg_data);

        MonitorStartup();
    }
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  InitWarm                                               **/
/**                                                                    **/
/**  Description:                                                      **/
/**     Init function for Warm boot                                    **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      None.                                                         **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**********************************END***********************************/
void PSM_HAL_OAM::InitWarm(void)
{
    HAL_INFO_PRINT("PSM_HAL_OAM: InitWarm() executing");
    isBootWarm = true;

    //initialize
    if(init_oam_fpga() != HAL_SUCCESS)  //need this to get fpgadriver
        //instance created
    {
        throw(HAL_ERROR);
    }
#ifdef __TARGET__
//  if(init_oam_arad() != HAL_SUCCESS)
//  {
//      throw(HAL_ERROR);
//  }
#endif //__TARGET__

    if (RestoreDb(OAM_DB_WB_FILE_FULLPATH) != HAL_SUCCESS) {
        HAL_INFO_PRINT("RestoreDb() failed - Forcing Cold Reboot\n");
        if ((remove(OAM_DB_WB_FILE_FULLPATH)) != 0) {
            HAL_INFO_PRINT("remove failed for %s", OAM_DB_WB_FILE_FULLPATH);
        }
        //throw exception to force reboot
        throw std::runtime_error("WarmFail");
    }
    //restore DB successful; now delete the wb file
    if ((remove(OAM_DB_WB_FILE_FULLPATH)) != 0) {
        HAL_INFO_PRINT("remove failed for %s", OAM_DB_WB_FILE_FULLPATH);
    }


    if(init_oam_debug() != HAL_SUCCESS) {
        throw(HAL_ERROR);
    }

    global.pm_cont_interval =
        HAL_SOAM_GLOBAL_DEF_PM_CONT_INTERVAL_MSEC;

    Tlab_utils_timer_cfg_data cfg_data;
    cfg_data.period = global.pm_cont_interval;
    Tlab_utils_timer_mgr_wait::Instance()->start(timer_id, &cfg_data);

    HAL_INFO_PRINT("disable oam monitoring\n");
    fpga_driver->enableOamCcCvFmProcessing(0);

    MonitorStartup();

    HAL_INFO_PRINT("PSM_HAL_OAM: InitWarm() complete");

}
/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  ShutdownWarm                                           **/
/**                                                                    **/
/**  Description:                                                      **/
/**     Shutdown function for Warm boot                                **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      None.                                                         **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**********************************END***********************************/
void PSM_HAL_OAM::ShutdownWarm(void)
{
    HAL_INFO_PRINT("PSM_HAL_OAM: ShutDownWarm() executing");
    //Stop any RFC2544 initiator running on this card
    if (stop_mep_enet_tp_initiator() != HAL_SUCCESS) {
        UTIL_ERR_MSG("warm reboot stop_mep_enet_tp_initiator() failed\n");
    }
    if (StoreDb(OAM_DB_WB_FILE_FULLPATH) != HAL_SUCCESS) {
        HAL_INFO_PRINT("warm reboot StoreDb() error - Forcing Cold Reboot\n");
        if ((remove(OAM_DB_WB_FILE_FULLPATH)) != 0) {
            HAL_WARN_PRINT("remove failed for %s", OAM_DB_WB_FILE_FULLPATH);
        }
        if ((remove("/tmp/WarmReset")) != 0) {
            HAL_WARN_PRINT("remove failed for %s", "/tmp/WarmReset");
        }
    }

    HAL_INFO_PRINT("PSM_HAL_OAM: ShutDownWarm() complete");
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  StoreDb                                                **/
/**                                                                    **/
/**  Description:                                                      **/
/**     Store OAM DB                                                   **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      None.                                                         **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::StoreDb(string filename)
{
//    int i;
//    HAL_PKT_DB_1_0 *BackupDb;

//    BackupDb = new HAL_PKT_DB_1_0;
//    BackupDb->version = 1;
    return PSM_HAL_OAM_OPE_DB_API_INST->StoreDb(filename);
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  Restore                                                **/
/**                                                                    **/
/**  Description:                                                      **/
/**     Restore OAM DB                                                 **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      None.                                                         **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      None.                                                         **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::RestoreDb(string filename)
{
//    int i;
//    HAL_PKT_DB_1_0 *BackupDb;
    return PSM_HAL_OAM_OPE_DB_API_INST->RestoreDb(filename);
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Create                                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will create a MEP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to create   **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Create(OAMHANDLE *handle, MEP_INFO *mep_info)
{
    OAMHANDLE* my_handle = NULL;

    //validate input pointers
    if((handle == NULL) || (mep_info == NULL) || (mep_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    e_HAL_SOAM_mp_tech_type mp_tech_type =
        static_cast<e_HAL_SOAM_mp_tech_type>(mep_info->header.mp_tech_type);

    if(mep_info->header.mp_type == e_ME) {
        PSM_HAL_OAM_OPE_DB_LOCK();

        //For Warm Boot
        //use the key and verify if it exists in the wb_map table; if it does, just return SUCCESS after filling in
        //the handle; this will handle the warm boot case
        t_HAL_SOAM_fpal_me_cfg* cfg_data =
            reinterpret_cast<t_HAL_SOAM_fpal_me_cfg*>(mep_info->buffer);

        PSM_HAL_OAM_wb_me_key key(cfg_data->u.enet.context_index, cfg_data->u.enet.md_index, cfg_data->u.enet.ma_index);

        if(FPAL_SOAM_IS_ENET_SERVICE(mp_tech_type)) {
            PSM_HAL_OAM_ope_entry_me* entry = NULL;
            if( PSM_HAL_OAM_OPE_DB_API_INST->get_me_map_entry(key, entry) == HAL_SUCCESS) {
                //fill in the handle and return
                handle->type = OAMHANDLE_ME;
                handle->id = entry->get_hal_id();

                PSM_HAL_OAM_OPE_DB_UNLOCK();

                if(oam_wb_print_flag) {
                    ostringstream ostr;
                    ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") me key:";
                    key.dump(ostr);
                    ostr<<"   is found, return hal_id:"<<handle->id<<endl;
                    HAL_INFO_PRINT(ostr.str().c_str());
                }
                return HAL_SUCCESS;
            }
        }

        if(HAL_ERROR == PSM_HAL_OAM_OPE_DB_API_INST->take(
                    e_ME, mp_tech_type, &my_handle)) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();

            ostringstream ostr;
            ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") "<<endl<<"\tme key:";
            key.dump_simple(ostr);
            ostr<<" fail to get hal_id"<<endl;
            UTIL_ERR_MSG(ostr.str().c_str());

            return HAL_ERROR;
        }

        if(me_oper(e_create, (my_handle->id),
                   mep_info) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_API_INST->give(e_ME,
                                              (my_handle->id));

            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }

        *handle = *my_handle;

        PSM_HAL_OAM_OPE_DB_UNLOCK();
    } else if(mep_info->header.mp_type == e_MEP) {
        PSM_HAL_OAM_OPE_DB_LOCK();

        //For Warm Boot
        //use the key and verify if it exists in the wb_map table; if it does, just return SUCCESS after filling in
        //the handle; this will handle the warm boot case
        t_HAL_SOAM_fpal_mep_cfg* cfg_data =
            reinterpret_cast<t_HAL_SOAM_fpal_mep_cfg*>(mep_info->buffer);
        uint32 tmp_me_hal_id = (uint32)-1;
        if(FPAL_SOAM_IS_ENET_SERVICE(mp_tech_type))
            tmp_me_hal_id = cfg_data->me_hal_id;
        PSM_HAL_OAM_wb_mep_key key(tmp_me_hal_id, cfg_data->mep_cp_id);
        PSM_HAL_OAM_ope_entry_mep* entry = NULL;
        if( PSM_HAL_OAM_OPE_DB_API_INST->get_mep_map_entry(key, entry) == HAL_SUCCESS) {
            handle->type = OAMHANDLE_MEP;
            handle->id = entry->get_hal_id();
            PSM_HAL_OAM_OPE_DB_UNLOCK();

            if(oam_wb_print_flag) {
                ostringstream ostr;
                ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") mep key:";
                key.dump(ostr);
                ostr<<"   is found, return hal_id:"<<handle->id<<endl;
                HAL_INFO_PRINT(ostr.str().c_str());
            }
            return HAL_SUCCESS;
        }

        if(HAL_ERROR == PSM_HAL_OAM_OPE_DB_API_INST->take(
                    e_MEP, mp_tech_type, &my_handle)) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();

            ostringstream ostr;
            ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") "<<endl<<"\tmep key:";
            key.dump_simple(ostr);
            ostr<<" fail to get hal_id"<<endl;
            UTIL_ERR_MSG(ostr.str().c_str());

            return HAL_ERROR;
        }

        if(mep_oper(e_create, (my_handle->id),
                    mep_info) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_API_INST->give(e_MEP,
                                              (my_handle->id));

            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }

        *handle = *my_handle;
        PSM_HAL_OAM_OPE_DB_UNLOCK();
    }

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Modify                                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will modify a MEP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to modify   **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Modify(OAMHANDLE *handle, MEP_INFO *mep_info)
{
    //validate input pointers
    if((handle == NULL) || (mep_info == NULL) || (mep_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    if(mep_info->header.mp_type == e_ME) {
        if(me_oper(e_modify, (handle->id),
                   mep_info) != HAL_SUCCESS) {
            return HAL_ERROR;
        }
    } else if(mep_info->header.mp_type == e_MEP) {
        uint8 oper = e_modify;

#if 0
        t_HAL_SOAM_fpal_mep_cfg* cfg_data =
            reinterpret_cast<t_HAL_SOAM_fpal_mep_cfg*>(mep_info->buffer);

        //this is a special case then MEP is delete in the HW and
        //not delete in the hal
        if(cfg_data->header.b_process_in_hal == true) {
            oper = e_modify;
        } else {
            oper = e_create;
        }
#endif

        PSM_HAL_OAM_OPE_DB_LOCK();

        if(mep_oper(oper, (handle->id),
                    mep_info) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }

        PSM_HAL_OAM_OPE_DB_UNLOCK();
    }

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Delete                                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will delete a MEP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to delete   **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Delete(OAMHANDLE *handle, MEP_INFO *mep_info)
{
    //validate input pointers
    if(handle == NULL) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
        UTIL_INF_MSG("id = %d", handle->id);
    }
#endif

    uint16 id = (handle->id);

    if(mep_info->header.mp_type == e_ME) {
        PSM_HAL_OAM_OPE_DB_LOCK();

        if(me_oper(e_delete, id, mep_info) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }

        if(PSM_HAL_OAM_OPE_DB_API_INST->give(e_ME, id) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();
            UTIL_ERR_MSG("give FAILED...");
            return HAL_ERROR;
        }

        PSM_HAL_OAM_OPE_DB_UNLOCK();
    } else if(mep_info->header.mp_type == e_MEP) {
        PSM_HAL_OAM_OPE_DB_LOCK();

        if(mep_oper(e_delete, id, mep_info) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }

        if(PSM_HAL_OAM_OPE_DB_API_INST->give(e_MEP, id) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }

        PSM_HAL_OAM_OPE_DB_UNLOCK();
    }

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Dump                                               **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will dump a MEP in the hardware                 **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Dump(OAMHANDLE *handle, MEP_INFO *mep_info)
{
    //validate input pointers
    if(handle == NULL) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    if(mep_info->header.mp_type == e_ME) {
        PSM_HAL_OAM_OPE_DB_LOCK();

        if(me_oper(e_dump, (handle->id),
                   mep_info) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }

        PSM_HAL_OAM_OPE_DB_UNLOCK();
    } else if(mep_info->header.mp_type == e_MEP) {
        PSM_HAL_OAM_OPE_DB_LOCK();

        if(mep_oper(e_dump, (handle->id),
                    mep_info) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }

        PSM_HAL_OAM_OPE_DB_UNLOCK();
    }

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  RMEP_Create                                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will create a RMEP in the hardware              **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the rmep information to use to create  **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::RMEP_Create(OAMHANDLE *handle, RMEP_INFO *rmep_info)
{
    OAMHANDLE* my_handle = NULL;

    //validate input pointers
    if((handle == NULL) || (rmep_info == NULL) || (rmep_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    e_HAL_SOAM_mp_tech_type mp_tech_type =
        static_cast<e_HAL_SOAM_mp_tech_type>(rmep_info->header.mp_tech_type);

    PSM_HAL_OAM_OPE_DB_LOCK();

    //For Warm Boot
    //use the key and verify if it exists in the wb_map table; if it does, just return SUCCESS after filling in
    //the handle; this will handle the warm boot case
    t_HAL_SOAM_fpal_rmep_cfg* cfg_data =
        reinterpret_cast<t_HAL_SOAM_fpal_rmep_cfg*>(rmep_info->buffer);

    uint32 tmp_rmep_id = uint32(-1);
    if(FPAL_SOAM_IS_ENET_SERVICE(mp_tech_type)) {
        tmp_rmep_id = cfg_data->rmep_cp_id;
    }

    PSM_HAL_OAM_wb_rmep_key key(cfg_data->mep_hal_id, tmp_rmep_id);

    PSM_HAL_OAM_ope_entry_rmep* entry = NULL;

    if( PSM_HAL_OAM_OPE_DB_API_INST->get_rmep_map_entry(key, entry) == HAL_SUCCESS) {
        handle->type = OAMHANDLE_RMEP;
        handle->id = entry->get_hal_id();
        PSM_HAL_OAM_OPE_DB_UNLOCK();

        if(oam_wb_print_flag) {
            ostringstream ostr;
            ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") rmep key:";
            key.dump(ostr);
            ostr<<"   is found, return hal_id:"<<handle->id<<endl;
            HAL_INFO_PRINT(ostr.str().c_str());
        }
        return HAL_SUCCESS;
    }

    if(HAL_ERROR == PSM_HAL_OAM_OPE_DB_API_INST->take(
                e_RMEP, mp_tech_type, &my_handle)) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();

        ostringstream ostr;
        ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") "<<endl<<"\trmep key:";
        key.dump_simple(ostr);
        ostr<<" fail to get hal_id"<<endl;
        UTIL_ERR_MSG(ostr.str().c_str());

        return HAL_ERROR;
    }

    if(rmep_oper(e_create, (my_handle->id),
                 rmep_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_API_INST->give(e_RMEP,
                                          (my_handle->id));

        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    *handle = *my_handle;

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  RMEP_Modify                                            **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will modify a RMEP in the hardware              **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the rmep information to use to modify  **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::RMEP_Modify(OAMHANDLE *handle, RMEP_INFO *rmep_info)
{
    //validate input pointers
    if((handle == NULL) || (rmep_info == NULL) || (rmep_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    uint8 oper = e_modify;

#if 0
    t_HAL_SOAM_fpal_rmep_cfg* cfg_data =
        reinterpret_cast<t_HAL_SOAM_fpal_rmep_cfg*>(rmep_info->buffer);

    //this is a special case then RMEP is delete in the HW and
    //not delete in the hal
    if(cfg_data->header.b_process_in_hal == true) {
        oper = e_modify;
    } else {
        oper = e_create;
    }
#endif

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(rmep_oper(oper, (handle->id),
                 rmep_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  RMEP_Delete                                            **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will delete a RMEP in the hardware              **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::RMEP_Delete(OAMHANDLE *handle, RMEP_INFO *rmep_info)
{
    //validate input pointers
    if(handle == NULL) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
        UTIL_INF_MSG("%d", handle->id);
    }
#endif

    t_HAL_SOAM_fpal_rmep_cfg* cfg_data =
        reinterpret_cast<t_HAL_SOAM_fpal_rmep_cfg*>(rmep_info->buffer);

    PSM_HAL_OAM_OPE_DB_LOCK();


    if(rmep_oper(e_delete, (handle->id),
                 rmep_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    if(cfg_data->header.b_process_in_hal == true) {
        if(PSM_HAL_OAM_OPE_DB_API_INST->give(e_RMEP,
                                             (handle->id)) != HAL_SUCCESS) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();

            UTIL_ERR_MSG("give FAILED...");
            return HAL_ERROR;
        }
    }

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  RMEP_Dump                                              **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will dump a RMEP in the hardware                **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::RMEP_Dump(OAMHANDLE *handle, RMEP_INFO *rmep_info)
{
    //validate input pointers
    if(handle == NULL) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(rmep_oper(e_dump, (handle->id),
                 rmep_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MIP_Create                                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will create a MIP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mip_info - pointer to the mip information to use to create   **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MIP_Create(OAMHANDLE *handle, MIP_INFO *mip_info)
{
    OAMHANDLE* my_handle = NULL;

    //validate input pointers
    if((handle == NULL) || (mip_info == NULL) || (mip_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    e_HAL_SOAM_mp_tech_type mp_tech_type =
        static_cast<e_HAL_SOAM_mp_tech_type>(mip_info->header.mp_tech_type);

    PSM_HAL_OAM_OPE_DB_LOCK();

    //For Warm Boot
    //use the key and verify if it exists in the wb_map table; if it does, just
    //return SUCCESS after filling in the handle; this will handle the warm boot case
    t_HAL_SOAM_fpal_mip_cfg* cfg_data =
        reinterpret_cast<t_HAL_SOAM_fpal_mip_cfg*>(mip_info->buffer);

    PSM_HAL_OAM_wb_mip_key key(cfg_data->lif_id, cfg_data->u.enet.md_level);
    PSM_HAL_OAM_ope_entry_mip* entry = NULL;

    if(FPAL_SOAM_IS_ENET_SERVICE(mp_tech_type)) {
        if( PSM_HAL_OAM_OPE_DB_API_INST->get_mip_map_entry(key, entry) == HAL_SUCCESS) {
            handle->type = OAMHANDLE_MIP;
            handle->id = entry->get_hal_id();
            PSM_HAL_OAM_OPE_DB_UNLOCK();

            if(oam_wb_print_flag) {
                ostringstream ostr;
                ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") mip key:";
                key.dump(ostr);
                ostr<<"   is found, return hal_id:"<<handle->id<<endl;
                HAL_INFO_PRINT(ostr.str().c_str());
            }
            return HAL_SUCCESS;
        }
    } else {
        ostringstream ostr;
        ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") "<<endl<<"mip key:";
        key.dump_simple(ostr);
        ostr<<" none ethernet mip is to be created!!!!"<<endl;
        UTIL_ERR_MSG(ostr.str().c_str());
    }

    if(HAL_ERROR == PSM_HAL_OAM_OPE_DB_API_INST->take(
                e_MIP, mp_tech_type, &my_handle)) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();

        ostringstream ostr;
        ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<")"<<endl<<"\tmip key:";
        key.dump_simple(ostr);
        ostr<<" fail to get hal_id"<<endl;
        UTIL_ERR_MSG(ostr.str().c_str());

        return HAL_ERROR;
    }

    if(mip_oper(e_create, (my_handle->id),
                mip_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_API_INST->give(e_MIP,
                                          (my_handle->id));

        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    *handle = *my_handle;

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MIP_Modify                                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will modify a MIP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mip_info - pointer to the mip information to use to modify   **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MIP_Modify(OAMHANDLE *handle, MIP_INFO *mip_info)
{
    //validate input pointers
    if((handle == NULL) || (mip_info == NULL) || (mip_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(mip_oper(e_modify, (handle->id),
                mip_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MIP_Delete                                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will delete a MIP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mip_info - pointer to the mip information to use to delete   **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MIP_Delete(OAMHANDLE *handle, MIP_INFO *mip_info)
{
    //validate input pointers
    if(handle == NULL) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
        UTIL_INF_MSG("id=%d", handle->id);
    }
#endif

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(mip_oper(e_delete, (handle->id),
                mip_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    if(PSM_HAL_OAM_OPE_DB_API_INST->give(e_MIP,
                                         (handle->id))) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        UTIL_ERR_MSG("give FAILED...");

        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MIP_Dump                                               **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will dump a MIP in the hardware                 **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MIP_Dump(OAMHANDLE *handle, MIP_INFO *mip_info)
{
    //validate input pointers
    if(handle == NULL) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(mip_oper(e_dump, (handle->id),
                mip_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Modify_AIS_Client                                  **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will create a MEP AIS CLIENT in the hardware    **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to create   **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Create_AIS_Client(OAMHANDLE *handle,
        MEP_AIS_CLIENT_INFO *mep_ais_client_info)
{
    OAMHANDLE* my_handle = NULL;

    //validate input pointers
    if((handle == NULL) || (mep_ais_client_info == NULL) ||
            (mep_ais_client_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    e_HAL_SOAM_mp_tech_type mp_tech_type =
        static_cast<e_HAL_SOAM_mp_tech_type>
        (mep_ais_client_info->header.mp_tech_type);

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
        //dump the data received from HAL
        UTIL_INF_MSG("");
        UTIL_dump_buffer(mep_ais_client_info->buffer,
                         mep_ais_client_info->length);
    }
#endif

    PSM_HAL_OAM_OPE_DB_LOCK();

    //for Warm boot
    //use the key and verify if it exists in the wb_map table; if it does, just return SUCCESS after filling in
    //the handle; this will handle the warm boot case
    t_HAL_SOAM_fpal_fm_client_cfg* cfg_data =
        reinterpret_cast<t_HAL_SOAM_fpal_fm_client_cfg*>(mep_ais_client_info->buffer);
    PSM_HAL_OAM_wb_fm_key key(cfg_data->context_index, cfg_data->md_index, cfg_data->ma_index, cfg_data->mep_id);
    PSM_HAL_OAM_ope_entry_fm* entry = NULL;

    if( PSM_HAL_OAM_OPE_DB_API_INST->get_fm_map_entry(key, entry) == HAL_SUCCESS) {
        handle->type = OAMHANDLE_FM;
        handle->id = entry->get_hal_id();
        PSM_HAL_OAM_OPE_DB_UNLOCK();

        if(oam_wb_print_flag) {
            ostringstream ostr;
            ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") fm key:";
            key.dump(ostr);
            ostr<<"   is found, return hal_id:"<<handle->id<<endl;
            HAL_INFO_PRINT(ostr.str().c_str());
        }
        return HAL_SUCCESS;
    }

    if(HAL_ERROR == PSM_HAL_OAM_OPE_DB_API_INST->take(
                e_FM_CLIENT, mp_tech_type, &my_handle)) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();

        ostringstream ostr;
        ostr <<__FILE__<<":"<<__LINE__<<"("<<__func__<<") "<<endl<<"\tais key:";
        key.dump_simple(ostr);
        ostr<<" fail to get hal_id"<<endl;
        UTIL_ERR_MSG(ostr.str().c_str());

        return HAL_ERROR;
    }

    if(fm_client_oper(e_create, e_AIS,
                      (my_handle->id),
                      mep_ais_client_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_API_INST->give(e_FM_CLIENT,
                                          (my_handle->id));

        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    *handle = *my_handle;

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Modify_AIS_Client                                  **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will create a MEP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to create   **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Modify_AIS_Client(OAMHANDLE *handle,
        MEP_AIS_CLIENT_INFO *mep_ais_client_info)
{
    //validate input pointers
    if((handle == NULL) || (mep_ais_client_info == NULL) ||
            (mep_ais_client_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
        //dump the data received from HAL
        UTIL_INF_MSG("");
        UTIL_dump_buffer(mep_ais_client_info->buffer, mep_ais_client_info->length);
    }
#endif

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(fm_client_oper(e_modify, e_AIS, (handle->id),
                      mep_ais_client_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    PSM_HAL_OAM_OPE_DB_UNLOCK();
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Delete                                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will delete a MEP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to delete   **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Delete_AIS_Client(OAMHANDLE *handle,
        MEP_AIS_CLIENT_INFO *mep_ais_client_info)
{
    //validate input pointers
    if(handle == NULL) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
        UTIL_INF_MSG("%d", handle->id);
    }
#endif

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(fm_client_oper(e_delete, e_AIS, (handle->id),
                      mep_ais_client_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    if(PSM_HAL_OAM_OPE_DB_API_INST->give(e_FM_CLIENT,
                                         (handle->id)) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        UTIL_ERR_MSG("give FAILED...");
        return HAL_ERROR;
    }
    PSM_HAL_OAM_OPE_DB_UNLOCK();

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Modify_LCK_Client                                  **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will create a MEP LCK CLIENT in the hardware    **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to create   **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Create_LCK_Client(OAMHANDLE *handle,
        MEP_LCK_CLIENT_INFO *mep_lck_client_info)
{
    OAMHANDLE* my_handle = NULL;

    //validate input pointers
    if((handle == NULL) || (mep_lck_client_info == NULL) ||
            (mep_lck_client_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    e_HAL_SOAM_mp_tech_type mp_tech_type =
        static_cast<e_HAL_SOAM_mp_tech_type>(
            mep_lck_client_info->header.mp_tech_type);

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
        //dump the data received from HAL
        UTIL_INF_MSG("");
        UTIL_dump_buffer(mep_lck_client_info->buffer,
                         mep_lck_client_info->length);
    }
#endif

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(HAL_ERROR == PSM_HAL_OAM_OPE_DB_API_INST->take(
                e_FM_CLIENT, mp_tech_type, &my_handle)) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    if(fm_client_oper(e_create, e_LCK,
                      (my_handle->id),
                      mep_lck_client_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_API_INST->give(e_FM_CLIENT,
                                          (my_handle->id));

        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    *handle = *my_handle;
    PSM_HAL_OAM_OPE_DB_UNLOCK();

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Modify_LCK_Client                                  **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will create a MEP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to create   **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Modify_LCK_Client(OAMHANDLE *handle,
        MEP_LCK_CLIENT_INFO *mep_lck_client_info)
{
    //validate input pointers
    if((handle == NULL) || (mep_lck_client_info == NULL) ||
            (mep_lck_client_info->buffer == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
        //dump the data received from HAL
        UTIL_INF_MSG("");
        UTIL_dump_buffer(mep_lck_client_info->buffer, mep_lck_client_info->length);
    }
#endif

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(fm_client_oper(e_modify, e_LCK, (handle->id),
                      mep_lck_client_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }
    PSM_HAL_OAM_OPE_DB_UNLOCK();

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Delete_LCK_Client                                  **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will delete a MEP in the hardware               **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**       mep_info - pointer to the mep information to use to delete   **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Delete_LCK_Client(OAMHANDLE *handle,
        MEP_LCK_CLIENT_INFO *mep_lck_client_info)
{
    //validate input pointers
    if(handle == NULL) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

#ifdef __SIMULATION__
    if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
        UTIL_INF_MSG("%d", handle->id);
    }
#endif

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(fm_client_oper(e_delete, e_LCK, (handle->id),
                      mep_lck_client_info) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }

    if(PSM_HAL_OAM_OPE_DB_API_INST->give(e_FM_CLIENT,
                                         (handle->id)) != HAL_ERROR) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        UTIL_ERR_MSG("give FAILED...");
        return HAL_ERROR;
    }
    PSM_HAL_OAM_OPE_DB_UNLOCK();

    return HAL_SUCCESS;
}

//-----------------------------------------------------------------------------
//        Class:  PSM_HAL_OAM
//
//       Method:  MEP_CC_Events_Set
//
//  Description:
//
//        Notes:
//-----------------------------------------------------------------------------
HAL_ERR PSM_HAL_OAM::MEP_CC_Events_Set(OAMHANDLE *handle,
                                       MEP_CC_EVENT_LIST  *events)
{
    (void)handle;
    (void)events;
    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  SOAM_Pdu_Xmt                                           **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will Xmt a Given PDU                            **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      pdu - pointer to the pdu to transmit                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::SOAM_Pdu_Xmt(OAMHANDLE *handle, MEP_PDU_TX *pdu,
                                  unsigned int size)
{
    (void)size;

    if((handle == NULL) || (pdu == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    return PSM_HAL_OAM_TX_FRAME_HANDLER_INST->send_frame_request(handle->id, pdu);
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  SOAM_Pdu_Get                                           **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will get the PDU                                **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      pdu - pointer to the pdu to transmit                          **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::SOAM_Pdu_Get(OAMHANDLE *handle, MEP_PDU_RX *input_pdu,
                                  MEP_PDU_RX *output_pdu, unsigned int *ret_size)
{
    memset(output_pdu, 0x00, sizeof(MEP_PDU_RX));
    *ret_size = 0;

    if(input_pdu->header.pdu_type == e_unknown_pdu) {
        PSM_HAL_OAM_OPE_DB_LOCK();

        if(PSM_HAL_oam_unknown_pkt_handler::Instance()->get_pkt(handle->id,
                output_pdu->pdu, (uint32*)ret_size) != TLAB_SUCCESS) {
            return HAL_ERROR;
        }
        PSM_HAL_OAM_OPE_DB_UNLOCK();

        memcpy(&(output_pdu->header), &(input_pdu->header), sizeof(MEP_PDU_HEADER));

        output_pdu->header.pdu_length = *ret_size;

        //reset the ret_size to the pud_len + header
        *ret_size = (*ret_size + sizeof(MEP_PDU_HEADER));

        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("");
            UTIL_dump_buffer((int8*)&(output_pdu->pdu),
                             output_pdu->header.pdu_length);
        }
    } else if(input_pdu->header.pdu_type == e_last_defect_pdu) {
        *ret_size = HAL_SOAM_MAX_PDU_LEN;

        PSM_HAL_OAM_OPE_DB_LOCK();

        if(fpga_driver->getOamRxLastDefectInfo(handle->id,
                                               (uint8*)&(output_pdu->pdu), (uint*)ret_size) != true) {
            PSM_HAL_OAM_OPE_DB_UNLOCK();
            return HAL_ERROR;
        }
        PSM_HAL_OAM_OPE_DB_UNLOCK();

        memcpy(&(output_pdu->header), &(input_pdu->header), sizeof(MEP_PDU_HEADER));

        output_pdu->header.pdu_length = *ret_size;

        //reset the ret_size to the pud_len + header
        *ret_size = (*ret_size + sizeof(MEP_PDU_HEADER));

        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("");
            UTIL_dump_buffer((int8*)&(output_pdu->pdu),
                             output_pdu->header.pdu_length);
        }
    }

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_Debug_Counts_Get                                   **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will Get a Given MEPs counts                    **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      counts - pointer to the counts                                **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_Debug_Counts_Get(OAMHANDLE *handle,
        MEP_DEBUG_COUNTERS *counts)
{
    //validate input pointers
    if((handle == NULL) || (counts == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    memset(counts, 0x00, sizeof(MEP_DEBUG_COUNTERS));

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(get_mep_counts_from_fpga(
                (handle->id), counts) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }
    PSM_HAL_OAM_OPE_DB_UNLOCK();

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MEP_BFD_session_data_Get                               **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will Get a Given MEPs data                      **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      handle - Pointer to the handle                                **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      data - pointer to the data                                    **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::MEP_BFD_session_data_Get(OAMHANDLE *handle,
        MEP_BFD_SESSION_DATA* data)
{
    //validate input pointers
    if((handle == NULL) || (data == NULL)) {
        UTIL_ERR_MSG_INVALID_INPUT();
        return HAL_ERROR;
    }

    memset(data, 0x00, sizeof(MEP_BFD_SESSION_DATA));

    PSM_HAL_OAM_OPE_DB_LOCK();

    if(get_mep_bfd_session_data_from_fpga(
                (handle->id), data) != HAL_SUCCESS) {
        PSM_HAL_OAM_OPE_DB_UNLOCK();
        return HAL_ERROR;
    }
    PSM_HAL_OAM_OPE_DB_UNLOCK();

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  global_info_set                                        **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will set OAM global info                        **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      global_info - pointer to the global info                      **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::global_info_set(HAL_OAM_GLOBAL_INFO* global_info)
{
    PSM_HAL_OAM_OPE_DB_LOCK();

    global.pm_cont_interval = global_info->global_cfg.pm_cont_interval;

    PSM_HAL_OAM_OPE_DB_UNLOCK();

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  MonTsk                                                 **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will be the task that monitors the states of the**/
/**     Monitor points.                                                **/
/**     Currently we are returning a value in an array.                **/
/**  Inputs:                                                           **/
/**      pArg - Not used.                                              **/
/**                                                                    **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**      none.                                                         **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      none.                                                         **/
/**                                                                    **/
/**********************************END***********************************/
void* PSM_HAL_OAM::MonTsk(void *pArg)
{
    (void)pArg;
    void *ret = NULL;

    while(1) {
#ifdef __SIMULATION__
        if(b_event_generate == true) {
            event_generator();
        } else {
            sleep(5);
        }
#else
        sleep(60);
#endif
    }

    return ret;
}

//-----------------------------------------------------------------------------
//        Class:  PSM_HAL_OAM
//
//       Method:  me_oper
//
//  Description:
//
//        Notes:
//-----------------------------------------------------------------------------
HAL_ERR PSM_HAL_OAM::me_oper(int8 oper, uint32 hal_id, MEP_INFO* mep_info)
{
    PSM_HAL_OAM_ope_entry_me* entry = NULL;

    uint16 id = (hal_id);

    if(NULL == (entry = (PSM_HAL_OAM_ope_entry_me*)(PSM_HAL_OAM_OPE_DB_API_INST->get_entry(e_ME, id)))) {
        UTIL_ERR_MSG("unable to add ME entry in DB,oper=%d,id=%d",oper,id);
        return HAL_ERROR;
    }

    if((oper == e_create) || (oper == e_modify)) { //note-modify will never come in for me entity; here for consistency
        t_HAL_SOAM_fpal_me_cfg* cfg_data =
            reinterpret_cast<t_HAL_SOAM_fpal_me_cfg*>(mep_info->buffer);

        //proceed only if required
        if(cfg_data->header.b_process_in_hw != true) {
            if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
                UTIL_INF_MSG("oper=%d,id = %d: will not process ME in the HW...",
                             oper, hal_id);
            }

            //we are done
            return HAL_SUCCESS;
        }

#ifdef __TARGET__
        int arad_id;
        if(create_me_in_arad(hal_id, oper, mep_info->header.mp_tech_type,
                             cfg_data, &arad_id) != HAL_SUCCESS) {
            return HAL_ERROR;
        }

        if(arad_id != TLAB_UTIL_INVALID_ENTITY) {
            entry->set_arad_oam_id(arad_id);
        }
#endif

        //on create, set the me_id to set up the warm boot map entry
        entry->set_me_id(cfg_data->u.enet.context_index, cfg_data->u.enet.md_index, cfg_data->u.enet.ma_index);

        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("ME: oper=%d, id=%d", oper, hal_id);
            entry->dump();
        }
    }
    //delete the entity
    else if(oper == e_delete) {
        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("");
            entry->dump();
        }

#ifdef __TARGET__
        if(entry->get_in_arad()) {
            if(delete_me_in_arad(entry->get_arad_oam_id()) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
        }
#endif

        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("delete ME (%d)", hal_id);
        }
    } else if(oper == e_dump) {
#ifdef __TARGET__
        entry->dump();

        if(dump_me_from_arad(hal_id) != HAL_SUCCESS) {
            return HAL_ERROR;
        }
#endif
    }

    return HAL_SUCCESS;
}

//-----------------------------------------------------------------------------
//        Class:  PSM_HAL_OAM
//
//       Method:  mep_oper
//
//  Description:
//
//        Notes:
//-----------------------------------------------------------------------------
HAL_ERR PSM_HAL_OAM::mep_oper(int8 oper, uint32 hal_id, MEP_INFO* mep_info)
{
    PSM_HAL_OAM_ope_entry* tmp_entry = NULL;
    PSM_HAL_OAM_ope_entry_mep* entry = NULL;
    uint16 outer_vlan;
    uint16 inner_vlan;
    uint8  outer_tpid;
    uint8  inner_tpid;

    t_HAL_SOAM_fpal_mep_cfg* cfg_data =
        reinterpret_cast<t_HAL_SOAM_fpal_mep_cfg*>(mep_info->buffer);

    e_HAL_SOAM_mp_tech_type mp_tech_type =
        static_cast<e_HAL_SOAM_mp_tech_type>(mep_info->header.mp_tech_type);

    if((oper == e_create) || (oper == e_modify) || (oper == e_delete)) {
        if(PSM_HAL_OAM_TRC_VERBOSE_INFO_ON||PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
            dump_mep_fpal_cfg_data(mp_tech_type, cfg_data);
        }
    }

    uint16 id = (hal_id);

    if(NULL == (tmp_entry = PSM_HAL_OAM_OPE_DB_API_INST->get_entry(e_MEP, id))) {
        UTIL_ERR_MSG("unable to get MEP entry in DB,oper=%d,id=%d",oper,id);
        return HAL_ERROR;
    }

    entry = reinterpret_cast<PSM_HAL_OAM_ope_entry_mep*>(tmp_entry);

    if ((FPAL_SOAM_IS_ENET_SERVICE(mp_tech_type)) &&
            ((oper == e_create) || (oper == e_modify))) {
        outer_vlan = cfg_data->u.enet.vlan_outer_tag.vid;
        inner_vlan = cfg_data->u.enet.vlan_inner_tag.vid;
        outer_tpid = (cfg_data->u.enet.vlan_outer_tag.tpid == 0x88a8) ? e_TPID_0x88a8: e_TPID_0x8100;
        inner_tpid = (cfg_data->u.enet.vlan_inner_tag.tpid == 0x88a8) ? e_TPID_0x88a8: e_TPID_0x8100;
        entry->set_outer_vlan(outer_vlan);
        entry->set_inner_vlan(inner_vlan);
        entry->set_outer_tpid(outer_tpid);
        entry->set_inner_tpid(inner_tpid);
    }

    //only when creating ENET MEP set up the ME for it.
    if(oper == e_create) {
        if(FPAL_SOAM_IS_ENET_SERVICE(mp_tech_type)) {
            if(entry->set_me_entry(cfg_data->me_hal_id) != HAL_SUCCESS) {
                UTIL_ERR_MSG("unable to add ME");
                return HAL_ERROR;
            }

            entry->set_vlan_tag_type(
                static_cast<e_HAL_SOAM_vlan_tag_type>
                (cfg_data->u.enet.vlan_tag_type));
            entry->set_mep_cp_id(cfg_data->mep_cp_id);
        } else {
            entry->set_mep_cp_id(cfg_data->mep_cp_id);
        }
    }


    //create/modify the entity
    if((oper == e_create) || (oper == e_modify)) {
        TLAB_UTILS_PROFILE_SAMPLE_START(1);


        //proceed only if required
        if(cfg_data->header.b_process_in_hw != true) {
            if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
                UTIL_INF_MSG("oper=%d,id = %d: will not process MEP in the HW...",
                             oper,hal_id);
            }

            //we are done
            return HAL_SUCCESS;
        }

        if(cfg_data->b_on_trunk_port == false) {
#if 0
            PSM_HAL_oam_ope_port* port_entry = PSM_HAL_OAM_OPE_DB_API_INST->
                                               get_port_entry(
                                                   HAL_SOAM_PORT_ENTRY_PORT_NUM(cfg_data->hw_port_num));

            if(port_entry->set_mac_addr(cfg_data->u.enet.src_mac_addr) != HAL_SUCCESS) {
                UTIL_ERR_MSG("unable to add mac address entry in port_table");
                return HAL_ERROR;
            }

            if(port_entry->add_mp(entry) != HAL_SUCCESS) {
                UTIL_ERR_MSG("unable to add entry in port_table");
                return HAL_ERROR;
            }
#endif
        } else {
            entry->set_on_lag(true);
            if (cfg_data->b_mc_link_is_set == true) {
                entry->set_is_lag_mc_link_present(true);
                entry->set_lag_mc_link_if_index(cfg_data->mc_link_if_index);
            } else {
                entry->set_is_lag_mc_link_present(false);
                entry->set_lag_mc_link_if_index(0);
            }
        }

        //no modification of MEP in arad is needed; only when creating
        //create the MEP in arad.
        if(oper == e_create) {
            TLAB_UTILS_PROFILE_SAMPLE_START(2);
#ifdef __TARGET__
            //create PMF entry for CEP UP MEP
            if (create_pmf_entry_for_cep_upmep(cfg_data) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
            //1- create MEP in ARAD

            int arad_id;
            if(create_mep_in_arad(entry, e_create, cfg_data, &arad_id) != HAL_SUCCESS) {
                return HAL_ERROR;
            }

            if(arad_id != TLAB_UTIL_INVALID_ENTITY) {
                entry->set_arad_oam_id(arad_id);
            }
#endif

            //save the MAC address to port mapping
            if(FPAL_SOAM_IS_ENET_SERVICE(mp_tech_type)) {
                if(PSM_HAL_OAM_OPE_DB_API_INST->add_mac_port_map_entry(
                            cfg_data->u.enet.src_mac_addr,
                            cfg_data->hw_port_num) != HAL_SUCCESS) {
                    UTIL_ERR_MSG("unable to add entry in mac_port_map,oper=%d,id=%d",oper,hal_id);
                    return HAL_ERROR;
                }


                if(PSM_HAL_OAM_OPE_DB_API_INST->add_lif_to_port_map_entry(
                            cfg_data->lif_id, cfg_data->hw_port_num) != HAL_SUCCESS) {
                    UTIL_ERR_MSG("unable to add entry in lif_to_port_map,lif_id=%d,hw_port_num=%d,oper=%d,id=%d",cfg_data->lif_id, cfg_data->hw_port_num,oper,hal_id);
                    return HAL_ERROR;
                }
            }

            TLAB_UTILS_PROFILE_SAMPLE_FINISH(2);
        }
//#endif
        TLAB_UTILS_PROFILE_SAMPLE_START(3);
        //2- create MEP in FPGA DB
        int fpga_id;
        if(create_mep_in_fpga_db(hal_id, oper, mp_tech_type,
                                 cfg_data, &fpga_id) != HAL_SUCCESS) {
            if(oper == e_create) {
#ifdef __TARGET__
                delete_mep_in_arad(entry->get_arad_oam_id(), mp_tech_type);
                entry->reset_arad_oam_id();
#endif
            }

            return HAL_ERROR;
        }

        //3- configure FPGA MEP control registers
        if(configure_mep_in_fpga_ctrl(entry, oper, cfg_data) != HAL_SUCCESS) {
            if(oper == e_create) {
#ifdef __TARGET__
                delete_mep_in_arad(entry->get_arad_oam_id(), mp_tech_type);
                entry->reset_arad_oam_id();
#endif
            }

            return HAL_ERROR;
        }

        if(!(entry->get_in_fpga()) && (fpga_id != TLAB_UTIL_INVALID_ENTITY)) {
            entry->set_in_fpga(true);
        }

        //save the admin state
        entry->set_admin_state(cfg_data->admin_state);
        entry->set_facing_up(cfg_data->b_up_mep);
        entry->set_hw_port_num(cfg_data->hw_port_num);
        entry->set_pg_associated_flag(cfg_data->b_pg_associated);

        TLAB_UTILS_PROFILE_SAMPLE_FINISH(3);
        TLAB_UTILS_PROFILE_SAMPLE_FINISH(1);

        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("MEP: oper=%d, id=%d", oper, hal_id);
            entry->dump();
        }
    }

    //delete the entity
    else if(oper == e_delete) {
        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("MEP: delete, id=%d - start",hal_id);
            entry->dump();
        }

        bool b_in_fpga = entry->get_in_fpga();

        //1- configure FPGA MEP control registers
        if(b_in_fpga) {
            if(configure_mep_in_fpga_ctrl(entry, oper, cfg_data) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
        }

#ifdef __TARGET__
        //2- delete MEP in ARAD
        if(entry->get_in_arad()) {
            if(delete_mep_in_arad(entry->get_arad_oam_id(), mp_tech_type) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
        }
#endif

        //3- delete MEP in FPGA DB
        if(b_in_fpga) {
            if(delete_mep_in_fpga_db(hal_id) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
            /* delete any TP ??*/
        }

#ifdef __TARGET__
        //delete PMF entry for CEP UP MEP
        if(delete_pmf_entry_for_cep_upmep(cfg_data) != HAL_SUCCESS) {
            return HAL_ERROR;
        }
#endif
        if((PSM_HAL_OAM_TRC_INFO_ON) || (PSM_HAL_OAM_EXPERT_TRC_INFO_ON)) {
            UTIL_INF_MSG("MEP: del (%d) - done", hal_id);
        }
    }

    //dump the entity
    else if(oper == e_dump) {
        entry->dump();

#ifdef __TARGET__
        if(dump_mep_from_arad(entry) != HAL_SUCCESS) {
            return HAL_ERROR;
        }
#endif

        if(dump_mep_from_fpga(hal_id) != HAL_SUCCESS) {
            return HAL_ERROR;
        }
    }

    return HAL_SUCCESS;
}

//-----------------------------------------------------------------------------
//        Class:  PSM_HAL_OAM
//
//       Method:  rmep_oper
//
//  Description:
//
//        Notes:
//-----------------------------------------------------------------------------
HAL_ERR PSM_HAL_OAM::rmep_oper(int8 oper, uint32 hal_id, RMEP_INFO* rmep_info)
{
    PSM_HAL_OAM_ope_entry* tmp_entry = NULL;
    PSM_HAL_OAM_ope_entry_rmep* entry = NULL;

    t_HAL_SOAM_fpal_rmep_cfg* cfg_data =
        reinterpret_cast<t_HAL_SOAM_fpal_rmep_cfg*>(rmep_info->buffer);

    e_HAL_SOAM_mp_tech_type mp_tech_type =
        static_cast<e_HAL_SOAM_mp_tech_type>(rmep_info->header.mp_tech_type);

    uint16 id = (hal_id);

    if(NULL == (tmp_entry = PSM_HAL_OAM_OPE_DB_API_INST->get_entry(e_RMEP, id))) {
        UTIL_ERR_MSG("unable to get RMEP entry in DB,oper=%d,id=%d",oper,id);
        return HAL_ERROR;
    }

    entry = reinterpret_cast<PSM_HAL_OAM_ope_entry_rmep*>(tmp_entry);

    if(oper == e_create) {
        if(entry->set_mep_entry(cfg_data->mep_hal_id) != HAL_SUCCESS) {
            UTIL_ERR_MSG("unable to set MEP,mep_hal_id=%d",cfg_data->mep_hal_id);
            return HAL_ERROR;
        }
    }

    if((oper == e_create) || (oper == e_modify) || (oper == e_delete)) {
        if(PSM_HAL_OAM_TRC_VERBOSE_INFO_ON||PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
            dump_rmep_fpal_cfg_data(mp_tech_type, cfg_data);
        }
    }

    //proceed only if required
    if(cfg_data->header.b_process_in_hw != true) {
        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("oper=%d,id = %d: will not process RMEP in the HW...",
                         oper,hal_id);
        }

        //we are done
        return HAL_SUCCESS;
    }

    //rmep count for the MEP
    uint16 rmep_count = entry->get_mep_entry()->get_rmep_count();

    if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
        UTIL_INF_MSG("For MEP (%d): Num of RMEPs (%d)",
                     entry->get_mep_entry()->get_hal_id(), rmep_count);
    }

    //create/modify the entity
    if((oper == e_create) || (oper == e_modify)) {
#ifdef __TARGET__
        //no modification of RMEP in arad is needed; only when creating
        //create the RMEP in arad.
        if(oper == e_create) {
            //create the first rmep in arad
            if(rmep_count == 1) {
                //1- create RMEP in ARAD
                int arad_id;
                if(create_rmep_in_arad(entry, oper, cfg_data, &arad_id) != HAL_SUCCESS) {
                    return HAL_ERROR;
                }

                if(arad_id != TLAB_UTIL_INVALID_ENTITY) {
                    entry->set_arad_oam_id(arad_id);
                }
            } else {
                if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
                    UTIL_INF_MSG("Only first RMEP in ARAD; "
                                 "MEP is multi-point; mep = %lx",
                                 (cfg_data->mep_hal_id));
                }

                //fill in the arad_id from the first that was created
                int arad_oam_id =
                    PSM_HAL_OAM_OPE_DB_API_INST->get_rmep_arad_id_for_rmep_in_arad(
                        (cfg_data->mep_hal_id));

                if(arad_oam_id == -1) {
                    UTIL_ERR_MSG("not a first RMEP and could not find ARAD oam id");
                    return HAL_ERROR;
                }

                entry->set_arad_oam_id(arad_oam_id, false);
            }
        } else {
        }
#endif

        //save the cp_id in the cb
        entry->set_rmep_cp_id(cfg_data->rmep_cp_id);

        //2- create RMEP in FPGA DB
        int fpga_id;
        if(create_rmep_in_fpga_db(hal_id, rmep_info->header.mp_tech_type,
                                  cfg_data, &fpga_id) != HAL_SUCCESS) {
            if((oper == e_create) && (entry->get_in_arad())) {
#ifdef __TARGET__
                delete_rmep_in_arad(entry->get_arad_oam_id(),
                                    mp_tech_type, cfg_data);
                entry->reset_arad_oam_id();
#endif
            }

            return HAL_ERROR;
        }

        //3- configure FPGA RMEP control registers
        if(configure_rmep_in_fpga_ctrl(entry, oper, cfg_data) != HAL_SUCCESS) {
            if((oper == e_create) && (entry->get_in_arad())) {
#ifdef __TARGET__
                delete_rmep_in_arad(entry->get_arad_oam_id(),
                                    mp_tech_type, cfg_data);
                entry->reset_arad_oam_id();
#endif
            }

            return HAL_ERROR;
        }

        if(!(entry->get_in_fpga()) && (fpga_id != TLAB_UTIL_INVALID_ENTITY)) {
            entry->set_in_fpga(true);
        }

        entry->set_admin_state(cfg_data->mep_admin_state);

        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("RMEP: oper=%d id=%d", oper, hal_id);
            entry->dump();
            entry->get_mep_entry()->dump();
        }
    }

    //delete the entity
    else if(oper == e_delete) {
        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("RMEP: del (%d) - start", hal_id);
            entry->dump();
        }

        bool b_in_fpga = entry->get_in_fpga();

        //1- configure FPGA RMEP control registers
        if(b_in_fpga) {
            if(configure_rmep_in_fpga_ctrl(entry, oper, cfg_data) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
        }

#ifdef __TARGET__
        //2- delete RMEP in ARAD
        if(entry->get_in_arad()) {
            if(delete_rmep_in_arad(entry->get_arad_oam_id(),
                                   mp_tech_type, cfg_data) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
            //In a Multipoint MEP when first RMEP with Arad being deleted
            if(rmep_count > 1) {
                //This was the first and only RMEP with ARAD ID configured
                //Reconfigure RMEP in Arad for next available REMEP
                PSM_HAL_OAM_ope_entry* tmp_mep_entry = NULL;

                if(NULL == (tmp_mep_entry = PSM_HAL_OAM_OPE_DB_API_INST->get_entry(e_MEP, cfg_data->mep_hal_id))) {
                    UTIL_ERR_MSG("unable to find entry");
                    return HAL_ERROR;
                }

                PSM_HAL_OAM_ope_entry_mep* mep_entry =
                    reinterpret_cast<PSM_HAL_OAM_ope_entry_mep*>(tmp_mep_entry);

                vector<PSM_HAL_OAM_ope_entry_rmep*> rmeps = mep_entry->get_rmeps();

                //find the RMEP which is different from the one being deleted
                vector<PSM_HAL_OAM_ope_entry_rmep*>::iterator it;

                for(it = rmeps.begin(); it != rmeps.end(); it++) {
                    //If HAL ID doesn't match, then use it for creating Arad RMEP ID
                    if((*it)->get_hal_id() != id) {
                        if(PSM_HAL_OAM_TRC_INFO_ON || PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
                            UTIL_INF_MSG("RMEP: Reconfiguring Arad RMEP for id %d", (*it)->get_hal_id());
                        }
                        int arad_id;
                        if(create_rmep_in_arad((*it), e_create, cfg_data, &arad_id) != HAL_SUCCESS) {
                            return HAL_ERROR;
                        }

                        if(arad_id != TLAB_UTIL_INVALID_ENTITY) {
                            (*it)->set_arad_oam_id(arad_id);
                        }
                        break;
                    }
                }

            }
        }
#endif

        //3- delete RMEP in FPGA DB
        if(b_in_fpga) {
            if(delete_rmep_in_fpga_db(hal_id) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
        }

        if(PSM_HAL_OAM_TRC_INFO_ON || PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("RMEP: del (%d) - done", hal_id);
        }
    }

    //dump the entity
    else if(oper == e_dump) {
        entry->dump();

#ifdef __TARGET__
        if(dump_rmep_from_arad(entry) != HAL_SUCCESS) {
            return HAL_ERROR;
        }
#endif

        if(dump_rmep_from_fpga(hal_id) != HAL_SUCCESS) {
            return HAL_ERROR;
        }
    }

    return HAL_SUCCESS;
}

//-----------------------------------------------------------------------------
//        Class:  PSM_HAL_OAM
//
//       Method:  fm_client_oper
//
//  Description:
//
//        Notes:
//-----------------------------------------------------------------------------
HAL_ERR PSM_HAL_OAM::fm_client_oper(int8 oper, int8 fm_type,
                                    uint32 hal_id, MEP_AIS_CLIENT_INFO* fm_client_info)
{
    uint8 fm_control;

    if((oper == e_create) || (oper == e_modify)) {
        t_HAL_SOAM_fm_client_db_data_buf db_data_buf;

        memset(&db_data_buf, 0x00, sizeof(t_HAL_SOAM_fm_client_db_data_buf));

        if(FPAL_SOAM_IS_ENET_SERVICE(fm_client_info->header.mp_tech_type)) {
            t_HAL_SOAM_fpal_fm_client_cfg* cfg_data =
                reinterpret_cast<t_HAL_SOAM_fpal_fm_client_cfg*>
                (fm_client_info->buffer);

            if(PSM_HAL_OAM_TRC_VERBOSE_INFO_ON||
                    PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
                dump_fm_client_fpal_cfg_data(
                    fm_client_info->header.mp_tech_type, cfg_data);
            }

            PSM_HAL_OAM_ope_entry_fm* entry = NULL;
            uint16 id = (hal_id);

            if(NULL == (entry = (PSM_HAL_OAM_ope_entry_fm*)(PSM_HAL_OAM_OPE_DB_API_INST->get_entry(e_FM_CLIENT, id)))) {
                UTIL_ERR_MSG("unable to find FM entry in DB,oper=%d,id=%d",oper,id);
                return HAL_ERROR;
            }

            if (oper == e_create) {
                //on create, set up server mep id to create the the warm boot map entry
                entry->set_fm_server_mep_id(cfg_data->context_index, cfg_data->md_index, cfg_data->ma_index, cfg_data->mep_id, cfg_data->lif_id);
            }

            //proceed only if required
            if(cfg_data->header.b_process_in_hw != true) {
                if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
                    UTIL_INF_MSG("oper=%d,id = %d: will not process FM in the HW...",
                                 oper, hal_id);
                }

                //we are done
                return HAL_SUCCESS;
            }

            //fill in the db entry
            if(build_enet_fm_client_db_entry(
                        fm_client_info->header.mp_tech_type,
                        fm_type, cfg_data, &db_data_buf) != HAL_SUCCESS) {
                return HAL_ERROR;
            }

            fm_control = cfg_data->fm_interval;
        } else if(FPAL_SOAM_IS_MPLS_TP_SERVICE(fm_client_info->header.mp_tech_type)) {
#ifndef qkd_TODO //fill this in
            fm_control = 0;
#endif
        } else {
            UTIL_ERR_MSG("unknown type (%d)",
                         fm_client_info->header.mp_tech_type);

            return HAL_ERROR;
        }

        if(PSM_HAL_OAM_EXPERT_TRC_FPGA_INFO_ON||PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
            UTIL_INF_MSG("id = %d", hal_id);

            if(PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
                UTIL_dump_buffer(reinterpret_cast<int8*>(db_data_buf.db_entry),
                                 HAL_SOAM_MAX_FM_CLIENT_DB_ENTRY_LEN);
            }
        }

        //call FPGA drive to insert FM client db entry in FPGA
        fpga_driver->setOamFmDbEntry(
            hal_id, db_data_buf.db_entry,
            HAL_SOAM_MAX_FM_CLIENT_DB_ENTRY_LEN);

        //validate that the entry has been created
        if(fpga_driver->isOamFmDbEntryEmpty(hal_id) == true) {
            UTIL_ERR_MSG("setOamFmDbEntry failed (id=%d)", hal_id);
            return HAL_ERROR;
        }

#ifdef qkd_TODO
        //call BCM SDK to install MEP entry in the sdk
#endif

        //call FPGA driver to configure MEP configuration in FPGA
        //1- control register
        fpga_driver->setOamEgressFmConfig(hal_id, fm_control);
    }
    //delete the entity
    else {
#ifdef qkd_TODO
        //remove the entry from arad
#endif

        //reset the configuration in FPGA
        //control register
        fpga_driver->setOamEgressFmConfig(hal_id, e_fm_control_diabled);

        //remove the entry in the FPGA FM client DB
        fpga_driver->deleteOamFmDbEntry(hal_id);

        //validate that the entry has been deleted
        if(fpga_driver->isOamFmDbEntryEmpty(hal_id) != true) {
            UTIL_ERR_MSG("isOamFmDbEntryEmpty failed (id=%d)", hal_id);
            return HAL_ERROR;
        }
    }

    return HAL_SUCCESS;
}

//-----------------------------------------------------------------------------
//        Class:  PSM_HAL_OAM
//
//       Method:  mip_oper
//
//  Description:
//
//        Notes:
//-----------------------------------------------------------------------------
HAL_ERR PSM_HAL_OAM::mip_oper(int8 oper, uint32 hal_id, MIP_INFO* mip_info)
{
    PSM_HAL_OAM_ope_entry* tmp_entry = NULL;
    PSM_HAL_OAM_ope_entry_mip* entry = NULL;

    uint16 id = (hal_id);

    if(NULL == (tmp_entry = PSM_HAL_OAM_OPE_DB_API_INST->get_entry(e_MIP, id))) {
        UTIL_ERR_MSG("unable to get MIP entry in DB,oper=%d,id=%d",oper, id);
        return HAL_ERROR;
    }
    entry = reinterpret_cast<PSM_HAL_OAM_ope_entry_mip*>(tmp_entry);

    //create/modify the entity
    if((oper == e_create) || (oper == e_modify)) {
        t_HAL_SOAM_fpal_mip_cfg* cfg_data =
            reinterpret_cast<t_HAL_SOAM_fpal_mip_cfg*>(mip_info->buffer);

        if(oper == e_create) {
            if(entry->set_me_entry(cfg_data->me_hal_id) != HAL_SUCCESS) {
                UTIL_ERR_MSG("unable to add ME,oper=%d,id=%d",oper,id);
                return HAL_ERROR;
            }
        }

        if(PSM_HAL_OAM_TRC_VERBOSE_INFO_ON||PSM_HAL_OAM_EXPERT_TRC_VERBOSE_INFO_ON) {
            UTIL_INF_MSG("oper = %d", oper);
            dump_mip_fpal_cfg_data(mip_info->header.mp_tech_type, cfg_data);
        }

        //proceed only if required
        if(cfg_data->header.b_process_in_hw != true) {
            if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
                UTIL_INF_MSG("oper=%d,id = %d: will not process MIP in the HW...",
                             oper, hal_id);
            }

            //we are done
            return HAL_SUCCESS;
        }

        if(cfg_data->b_on_trunk_port == false) {
#if 0
            PSM_HAL_oam_ope_port* port_entry = PSM_HAL_OAM_OPE_DB_API_INST->
                                               get_port_entry(cfg_data->hw_port_num);

            if(port_entry->set_mac_addr(cfg_data->u.enet.src_mac_addr) != HAL_SUCCESS) {
                UTIL_ERR_MSG("unable to add mac address entry in port_table");
                return HAL_ERROR;
            }

            if(port_entry->add_mp(entry) != HAL_SUCCESS) {
                UTIL_ERR_MSG("unable to add entry in port_table");
                return HAL_ERROR;
            }
#endif
        } else {
            entry->set_on_lag(true);
        }


        int arad_id;
#ifdef __TARGET__
        //create MIP in ARAD
        if(create_mip_in_arad(entry, oper, cfg_data, &arad_id) != HAL_SUCCESS) {
            return HAL_ERROR;
        }

        if(arad_id != TLAB_UTIL_INVALID_ENTITY) {
            entry->set_arad_oam_id(arad_id);
        }
#endif

        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("MIP: oper=%d,id=%d,arad_oam_id=0x%x", oper, hal_id, arad_id);
            entry->dump();
        }

        //save the MAC address to port mapping
        if((oper == e_create) &&
                (FPAL_SOAM_IS_ENET_SERVICE(mip_info->header.mp_tech_type))) {
            if(PSM_HAL_OAM_OPE_DB_API_INST->add_mac_port_map_entry(
                        cfg_data->u.enet.src_mac_addr,
                        cfg_data->hw_port_num) != HAL_SUCCESS) {
                UTIL_ERR_MSG("unable to add entry in mac_port_map,oper=%d,id=%d",oper,hal_id);
                return HAL_ERROR;
            }


            if(PSM_HAL_OAM_OPE_DB_API_INST->add_lif_to_port_map_entry(
                        cfg_data->lif_id, cfg_data->hw_port_num) != HAL_SUCCESS) {
                UTIL_ERR_MSG("unable to add entry in lif_to_port_map,oper=%d,hal_id=%d",oper,hal_id);
                return HAL_ERROR;
            }
            //on create, set the lif and md_level to create the warm boot map entry
            entry->set_lif_id(cfg_data->lif_id);
            entry->set_md_level(cfg_data->u.enet.md_level);
        }
        if(PSM_HAL_OAM_TRC_INFO_ON) {
            UTIL_INF_MSG("MIP: oper=%d,id=%d,arad_oam_id=%d", oper, hal_id, arad_id);
        }
    }

    //delete the entity
    else if(oper == e_delete) {
        if(PSM_HAL_OAM_EXPERT_TRC_INFO_ON) {
            UTIL_INF_MSG("");
            entry->dump();
        }

#ifdef __TARGET__
        if(entry->get_in_arad()) {
            if(delete_mip_in_arad(entry->get_arad_oam_id()) != HAL_SUCCESS) {
                return HAL_ERROR;
            }
        }
#endif
        if(PSM_HAL_OAM_TRC_INFO_ON) {
            UTIL_INF_MSG("MIP: del (%d)", hal_id);
        }
    }

    //dump the entity
    else if(oper == e_dump) {
#ifdef __TARGET__
        entry->dump();

        if(dump_mip_from_arad(hal_id) != HAL_SUCCESS) {
            return HAL_ERROR;
        }
#endif
    }

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  sync_info_set                                          **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will set OAM sync info                          **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      sync_info - pointer to the sync info                          **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::sync_info_set(HAL_OAM_SYNC_INFO* sync_info)
{
    //(void*) sync_info;
    HAL_INFO_PRINT("sync_done, isBootWarm: %d\n", isBootWarm);

    if( isBootWarm ) {
        HAL_INFO_PRINT("enable oam monitoring\n");
        fpga_driver->enableOamCcCvFmProcessing(1);
    }

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  rfc2544_warm_init_data_get                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will get data to be stored during warm init for **/
/**      RFC2544                                                       **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**        None                                                        **/
/**  Outputs:                                                          **/
/**      lbrIndex     - LBR Index Mask                                 **/
/**      state        - rfc2544 state of the initiator                 **/
/**      oam_ev       - OAM Event data                                 **/
/**                                                                    **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::rfc2544_warm_init_data_get(uint16 *lbrIndex,
        struct Devices::Fpga::rfc2544State *state,
        HAL_OAM_EVENT_DATA  *oam_ev)
{
    if ((lbrIndex == NULL) || (state == NULL) || (oam_ev == NULL)) {
        return HAL_ERROR;
    }

    *lbrIndex = lbrIndexMask;
    memcpy(state, &state2544, sizeof(struct Devices::Fpga::rfc2544State));
    memcpy(oam_ev, &oam_ev_data, sizeof(HAL_OAM_EVENT_DATA));

    return HAL_SUCCESS;
}

/*********************************BEGIN**********************************/
/**                                                                    **/
/**  Function:  rfc2544_warm_init_data_set                             **/
/**                                                                    **/
/**  Description:                                                      **/
/**      This function will set data stored during warm init for       **/
/**      RFC2544                                                       **/
/**                                                                    **/
/**  Inputs:                                                           **/
/**      lbrIndex     - LBR Index Mask                                 **/
/**      state        - rfc2544 state of the initiator                 **/
/**      oam_ev       - OAM Event data                                 **/
/**                                                                    **/
/**  Outputs:                                                          **/
/**                                                                    **/
/**  Returns:                                                          **/
/**      HAL_ERR - Error status.                                       **/
/**                                                                    **/
/**********************************END***********************************/
HAL_ERR PSM_HAL_OAM::rfc2544_warm_init_data_set(uint16 lbrIndex,
        struct Devices::Fpga::rfc2544State *state,
        HAL_OAM_EVENT_DATA  *oam_ev)
{
    if ((state == NULL) || (oam_ev == NULL)) {
        return HAL_ERROR;
    }

    lbrIndexMask = lbrIndex;
    memcpy(&state2544, state, sizeof(struct Devices::Fpga::rfc2544State));
    memcpy(&oam_ev_data, oam_ev, sizeof(HAL_OAM_EVENT_DATA));

    return HAL_SUCCESS;
}
