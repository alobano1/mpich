/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2006 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 *
 *  Portions of this code were written by Intel Corporation.
 *  Copyright (C) 2011-2016 Intel Corporation.  Intel provides this material
 *  to Argonne National Laboratory subject to Software Grant and Corporate
 *  Contributor License Agreement dated February 8, 2012.
 */
#ifndef CH4_PROBE_H_INCLUDED
#define CH4_PROBE_H_INCLUDED

#include "ch4_impl.h"

#undef FUNCNAME
#define FUNCNAME MPIDI_Probe
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
MPL_STATIC_INLINE_PREFIX int MPIDI_Probe(int source,
                                         int tag, MPIR_Comm * comm, int context_offset,
                                         MPI_Status * status)
{
    int mpi_errno, flag = 0;
    MPIR_FUNC_VERBOSE_STATE_DECL(MPID_STATE_MPIDI_PROBE);
    MPIR_FUNC_VERBOSE_ENTER(MPID_STATE_MPIDI_PROBE);

    if (unlikely(source == MPI_PROC_NULL)) {
        MPIR_Status_set_procnull(status);
        mpi_errno = MPI_SUCCESS;
        goto fn_exit;
    }

    while (!flag) {
#ifndef MPIDI_CH4_EXCLUSIVE_SHM
        mpi_errno = MPIDI_NM_mpi_iprobe(source, tag, comm, context_offset, &flag, status);
#else
        if (unlikely(source == MPI_ANY_SOURCE)) {
            mpi_errno = MPIDI_SHM_mpi_iprobe(source, tag, comm, context_offset, &flag, status);
            if (!flag)
                mpi_errno = MPIDI_NM_mpi_iprobe(source, tag, comm, context_offset, &flag, status);
        }
        else if (MPIDI_CH4_rank_is_local(source, comm))
            mpi_errno = MPIDI_SHM_mpi_iprobe(source, tag, comm, context_offset, &flag, status);
        else
            mpi_errno = MPIDI_NM_mpi_iprobe(source, tag, comm, context_offset, &flag, status);
#endif
        if (mpi_errno != MPI_SUCCESS) {
            MPIR_ERR_POP(mpi_errno);
        }
        MPIDI_Progress_test();
    }
  fn_exit:
    MPIR_FUNC_VERBOSE_EXIT(MPID_STATE_MPIDI_PROBE);
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}


#undef FUNCNAME
#define FUNCNAME MPIDI_Mprobe
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
MPL_STATIC_INLINE_PREFIX int MPIDI_Mprobe(int source,
                                          int tag,
                                          MPIR_Comm * comm,
                                          int context_offset, MPIR_Request ** message,
                                          MPI_Status * status)
{
    int mpi_errno = MPI_SUCCESS, flag = 0;
    MPIR_FUNC_VERBOSE_STATE_DECL(MPID_STATE_MPIDI_CH4U_MPROBE);
    MPIR_FUNC_VERBOSE_ENTER(MPID_STATE_MPIDI_CH4U_MPROBE);

    if (source == MPI_PROC_NULL) {
        MPIR_Status_set_procnull(status);
        *message = NULL;        /* should be interpreted as MPI_MESSAGE_NO_PROC */
        mpi_errno = MPI_SUCCESS;
        goto fn_exit;
    }

    while (!flag) {
#ifndef MPIDI_CH4_EXCLUSIVE_SHM
        mpi_errno = MPIDI_NM_mpi_improbe(source, tag, comm, context_offset, &flag, message, status);
#else
        if (unlikely(source == MPI_ANY_SOURCE)) {
            mpi_errno =
                MPIDI_SHM_mpi_improbe(source, tag, comm, context_offset, &flag, message, status);
            if (!flag)
                mpi_errno =
                    MPIDI_NM_mpi_improbe(source, tag, comm, context_offset, &flag, message, status);
        }
        else if (MPIDI_CH4_rank_is_local(source, comm))
            mpi_errno =
                MPIDI_SHM_mpi_improbe(source, tag, comm, context_offset, &flag, message, status);
        else
            mpi_errno =
                MPIDI_NM_mpi_improbe(source, tag, comm, context_offset, &flag, message, status);
#endif
        if (mpi_errno != MPI_SUCCESS) {
            MPIR_ERR_POP(mpi_errno);
        }
        MPIDI_Progress_test();
    }
  fn_exit:
    MPIR_FUNC_VERBOSE_EXIT(MPID_STATE_MPIDI_CH4U_MPROBE);
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}

#undef FUNCNAME
#define FUNCNAME MPIDI_Improbe
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
MPL_STATIC_INLINE_PREFIX int MPIDI_Improbe(int source,
                                           int tag,
                                           MPIR_Comm * comm,
                                           int context_offset,
                                           int *flag, MPIR_Request ** message, MPI_Status * status)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_FUNC_VERBOSE_STATE_DECL(MPID_STATE_MPIDI_IMPROBE);
    MPIR_FUNC_VERBOSE_ENTER(MPID_STATE_MPIDI_IMPROBE);

    if (source == MPI_PROC_NULL) {
        MPIR_Status_set_procnull(status);
        *flag = 1;
        *message = NULL;        /* should be interpreted as MPI_MESSAGE_NO_PROC */
        mpi_errno = MPI_SUCCESS;
        goto fn_exit;
    }

#ifndef MPIDI_CH4_EXCLUSIVE_SHM
    mpi_errno = MPIDI_NM_mpi_improbe(source, tag, comm, context_offset, flag, message, status);
#else
    if (unlikely(source == MPI_ANY_SOURCE)) {
        mpi_errno = MPIDI_SHM_mpi_improbe(source, tag, comm, context_offset, flag, message, status);
        if (!*flag)
            mpi_errno =
                MPIDI_NM_mpi_improbe(source, tag, comm, context_offset, flag, message, status);
    }
    else if (MPIDI_CH4_rank_is_local(source, comm))
        mpi_errno = MPIDI_SHM_mpi_improbe(source, tag, comm, context_offset, flag, message, status);
    else
        mpi_errno = MPIDI_NM_mpi_improbe(source, tag, comm, context_offset, flag, message, status);
#endif
    if (mpi_errno != MPI_SUCCESS) {
        MPIR_ERR_POP(mpi_errno);
    }
  fn_exit:
    MPIR_FUNC_VERBOSE_EXIT(MPID_STATE_MPIDI_IMPROBE);
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}

#undef FUNCNAME
#define FUNCNAME MPIDI_Iprobe
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
MPL_STATIC_INLINE_PREFIX int MPIDI_Iprobe(int source,
                                          int tag,
                                          MPIR_Comm * comm,
                                          int context_offset, int *flag, MPI_Status * status)
{

    int mpi_errno;
    MPIR_FUNC_VERBOSE_STATE_DECL(MPID_STATE_MPIDI_IPROBE);
    MPIR_FUNC_VERBOSE_ENTER(MPID_STATE_MPIDI_IPROBE);

    if (unlikely(source == MPI_PROC_NULL)) {
        MPIR_Status_set_procnull(status);
        *flag = 1;
        mpi_errno = MPI_SUCCESS;
        goto fn_exit;
    }

#ifndef MPIDI_CH4_EXCLUSIVE_SHM
    mpi_errno = MPIDI_NM_mpi_iprobe(source, tag, comm, context_offset, flag, status);
#else
    if (unlikely(source == MPI_ANY_SOURCE)) {
        mpi_errno = MPIDI_SHM_mpi_iprobe(source, tag, comm, context_offset, flag, status);
        if (!*flag)
            mpi_errno = MPIDI_NM_mpi_iprobe(source, tag, comm, context_offset, flag, status);
    }
    else if (MPIDI_CH4_rank_is_local(source, comm))
        mpi_errno = MPIDI_SHM_mpi_iprobe(source, tag, comm, context_offset, flag, status);
    else
        mpi_errno = MPIDI_NM_mpi_iprobe(source, tag, comm, context_offset, flag, status);
#endif
    if (mpi_errno != MPI_SUCCESS) {
        MPIR_ERR_POP(mpi_errno);
    }
  fn_exit:
    MPIR_FUNC_VERBOSE_EXIT(MPID_STATE_MPIDI_IPROBE);
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}

#endif /* CH4_PROBE_H_INCLUDED */
