/*
 *  SaaS/SdkFeatures.h
 *  libsoftphone
 *
 *  Copyright (c) 2013 - 2016 Acrobits, s.r.o.. All rights reserved.
 *
 */

#pragma once

/** @brief Enables support for add-ons */
#define SOFTPHONE_ADDONS

/** @brief Enables balance service */
#define SOFTPHONE_BALANCE

/** @brief Enables call-through mechanism */
#define SOFTPHONE_CALLTHROUGH

/** @brief Enables conference calls */
#define SOFTPHONE_CONFERENCES

/** @brief Enables messaging */
#define SOFTPHONE_MESSAGING

/** @brief Enables multiple accounts */
 #define SOFTPHONE_MULTIPLE_ACCOUNTS

/** @brief Enables number rewriting and address book matching */
#define SOFTPHONE_NUMBER_REWRITING

/** @brief Enables XML provisioning */
#define SOFTPHONE_PROVISIONING

/** @brief Enables support for push notifications */
#define SOFTPHONE_PUSH

/** @brief Enables call recording */
#define SOFTPHONE_RECORD

/** @brief Enables call and message encryption */
#define SOFTPHONE_SECURITY

#if defined(SOFTPHONE_SECURITY)
/** @brief Enables ZRTP call encryption */
#   define SOFTPHONE_SECURITY_ZRTP
#endif

/** @brief Enables tracking engine configuration */
//#define SOFTPHONE_TRACKING

/** @brief Enables TSC tunnel functionality */
//#define SOFTPHONE_TSCF

/** @brief Enables video calls */
#define SOFTPHONE_VIDEO

/** @brief Enables voicemail */
#define SOFTPHONE_VOICEMAIL

/** @brief Enables web callback mechanism */
#define SOFTPHONE_WEB_CALLBACK

/** @brief Enables "smart contacts" */
#define SOFTPHONE_SMART_CONTACTS
