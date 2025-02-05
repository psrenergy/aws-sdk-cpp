﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/lambda/Lambda_EXPORTS.h>
#include <aws/lambda/model/ProvisionedConcurrencyStatusEnum.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
template<typename RESULT_TYPE>
class AmazonWebServiceResult;

namespace Utils
{
namespace Json
{
  class JsonValue;
} // namespace Json
} // namespace Utils
namespace Lambda
{
namespace Model
{
  class AWS_LAMBDA_API GetProvisionedConcurrencyConfigResult
  {
  public:
    GetProvisionedConcurrencyConfigResult();
    GetProvisionedConcurrencyConfigResult(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);
    GetProvisionedConcurrencyConfigResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);


    /**
     * <p>The amount of provisioned concurrency requested.</p>
     */
    inline int GetRequestedProvisionedConcurrentExecutions() const{ return m_requestedProvisionedConcurrentExecutions; }

    /**
     * <p>The amount of provisioned concurrency requested.</p>
     */
    inline void SetRequestedProvisionedConcurrentExecutions(int value) { m_requestedProvisionedConcurrentExecutions = value; }

    /**
     * <p>The amount of provisioned concurrency requested.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithRequestedProvisionedConcurrentExecutions(int value) { SetRequestedProvisionedConcurrentExecutions(value); return *this;}


    /**
     * <p>The amount of provisioned concurrency available.</p>
     */
    inline int GetAvailableProvisionedConcurrentExecutions() const{ return m_availableProvisionedConcurrentExecutions; }

    /**
     * <p>The amount of provisioned concurrency available.</p>
     */
    inline void SetAvailableProvisionedConcurrentExecutions(int value) { m_availableProvisionedConcurrentExecutions = value; }

    /**
     * <p>The amount of provisioned concurrency available.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithAvailableProvisionedConcurrentExecutions(int value) { SetAvailableProvisionedConcurrentExecutions(value); return *this;}


    /**
     * <p>The amount of provisioned concurrency allocated. When a weighted alias is
     * used during linear and canary deployments, this value fluctuates depending on
     * the amount of concurrency that is provisioned for the function versions.</p>
     */
    inline int GetAllocatedProvisionedConcurrentExecutions() const{ return m_allocatedProvisionedConcurrentExecutions; }

    /**
     * <p>The amount of provisioned concurrency allocated. When a weighted alias is
     * used during linear and canary deployments, this value fluctuates depending on
     * the amount of concurrency that is provisioned for the function versions.</p>
     */
    inline void SetAllocatedProvisionedConcurrentExecutions(int value) { m_allocatedProvisionedConcurrentExecutions = value; }

    /**
     * <p>The amount of provisioned concurrency allocated. When a weighted alias is
     * used during linear and canary deployments, this value fluctuates depending on
     * the amount of concurrency that is provisioned for the function versions.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithAllocatedProvisionedConcurrentExecutions(int value) { SetAllocatedProvisionedConcurrentExecutions(value); return *this;}


    /**
     * <p>The status of the allocation process.</p>
     */
    inline const ProvisionedConcurrencyStatusEnum& GetStatus() const{ return m_status; }

    /**
     * <p>The status of the allocation process.</p>
     */
    inline void SetStatus(const ProvisionedConcurrencyStatusEnum& value) { m_status = value; }

    /**
     * <p>The status of the allocation process.</p>
     */
    inline void SetStatus(ProvisionedConcurrencyStatusEnum&& value) { m_status = std::move(value); }

    /**
     * <p>The status of the allocation process.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithStatus(const ProvisionedConcurrencyStatusEnum& value) { SetStatus(value); return *this;}

    /**
     * <p>The status of the allocation process.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithStatus(ProvisionedConcurrencyStatusEnum&& value) { SetStatus(std::move(value)); return *this;}


    /**
     * <p>For failed allocations, the reason that provisioned concurrency could not be
     * allocated.</p>
     */
    inline const Aws::String& GetStatusReason() const{ return m_statusReason; }

    /**
     * <p>For failed allocations, the reason that provisioned concurrency could not be
     * allocated.</p>
     */
    inline void SetStatusReason(const Aws::String& value) { m_statusReason = value; }

    /**
     * <p>For failed allocations, the reason that provisioned concurrency could not be
     * allocated.</p>
     */
    inline void SetStatusReason(Aws::String&& value) { m_statusReason = std::move(value); }

    /**
     * <p>For failed allocations, the reason that provisioned concurrency could not be
     * allocated.</p>
     */
    inline void SetStatusReason(const char* value) { m_statusReason.assign(value); }

    /**
     * <p>For failed allocations, the reason that provisioned concurrency could not be
     * allocated.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithStatusReason(const Aws::String& value) { SetStatusReason(value); return *this;}

    /**
     * <p>For failed allocations, the reason that provisioned concurrency could not be
     * allocated.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithStatusReason(Aws::String&& value) { SetStatusReason(std::move(value)); return *this;}

    /**
     * <p>For failed allocations, the reason that provisioned concurrency could not be
     * allocated.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithStatusReason(const char* value) { SetStatusReason(value); return *this;}


    /**
     * <p>The date and time that a user last updated the configuration, in <a
     * href="https://www.iso.org/iso-8601-date-and-time-format.html">ISO 8601
     * format</a>.</p>
     */
    inline const Aws::String& GetLastModified() const{ return m_lastModified; }

    /**
     * <p>The date and time that a user last updated the configuration, in <a
     * href="https://www.iso.org/iso-8601-date-and-time-format.html">ISO 8601
     * format</a>.</p>
     */
    inline void SetLastModified(const Aws::String& value) { m_lastModified = value; }

    /**
     * <p>The date and time that a user last updated the configuration, in <a
     * href="https://www.iso.org/iso-8601-date-and-time-format.html">ISO 8601
     * format</a>.</p>
     */
    inline void SetLastModified(Aws::String&& value) { m_lastModified = std::move(value); }

    /**
     * <p>The date and time that a user last updated the configuration, in <a
     * href="https://www.iso.org/iso-8601-date-and-time-format.html">ISO 8601
     * format</a>.</p>
     */
    inline void SetLastModified(const char* value) { m_lastModified.assign(value); }

    /**
     * <p>The date and time that a user last updated the configuration, in <a
     * href="https://www.iso.org/iso-8601-date-and-time-format.html">ISO 8601
     * format</a>.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithLastModified(const Aws::String& value) { SetLastModified(value); return *this;}

    /**
     * <p>The date and time that a user last updated the configuration, in <a
     * href="https://www.iso.org/iso-8601-date-and-time-format.html">ISO 8601
     * format</a>.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithLastModified(Aws::String&& value) { SetLastModified(std::move(value)); return *this;}

    /**
     * <p>The date and time that a user last updated the configuration, in <a
     * href="https://www.iso.org/iso-8601-date-and-time-format.html">ISO 8601
     * format</a>.</p>
     */
    inline GetProvisionedConcurrencyConfigResult& WithLastModified(const char* value) { SetLastModified(value); return *this;}

  private:

    int m_requestedProvisionedConcurrentExecutions;

    int m_availableProvisionedConcurrentExecutions;

    int m_allocatedProvisionedConcurrentExecutions;

    ProvisionedConcurrencyStatusEnum m_status;

    Aws::String m_statusReason;

    Aws::String m_lastModified;
  };

} // namespace Model
} // namespace Lambda
} // namespace Aws
