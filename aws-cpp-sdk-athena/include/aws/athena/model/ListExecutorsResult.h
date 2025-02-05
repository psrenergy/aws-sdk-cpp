﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/athena/Athena_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/athena/model/ExecutorsSummary.h>
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
namespace Athena
{
namespace Model
{
  class AWS_ATHENA_API ListExecutorsResult
  {
  public:
    ListExecutorsResult();
    ListExecutorsResult(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);
    ListExecutorsResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);


    /**
     * <p>The session ID.</p>
     */
    inline const Aws::String& GetSessionId() const{ return m_sessionId; }

    /**
     * <p>The session ID.</p>
     */
    inline void SetSessionId(const Aws::String& value) { m_sessionId = value; }

    /**
     * <p>The session ID.</p>
     */
    inline void SetSessionId(Aws::String&& value) { m_sessionId = std::move(value); }

    /**
     * <p>The session ID.</p>
     */
    inline void SetSessionId(const char* value) { m_sessionId.assign(value); }

    /**
     * <p>The session ID.</p>
     */
    inline ListExecutorsResult& WithSessionId(const Aws::String& value) { SetSessionId(value); return *this;}

    /**
     * <p>The session ID.</p>
     */
    inline ListExecutorsResult& WithSessionId(Aws::String&& value) { SetSessionId(std::move(value)); return *this;}

    /**
     * <p>The session ID.</p>
     */
    inline ListExecutorsResult& WithSessionId(const char* value) { SetSessionId(value); return *this;}


    /**
     * <p>A token generated by the Athena service that specifies where to continue
     * pagination if a previous request was truncated. To obtain the next set of pages,
     * pass in the <code>NextToken</code> from the response object of the previous page
     * call.</p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>A token generated by the Athena service that specifies where to continue
     * pagination if a previous request was truncated. To obtain the next set of pages,
     * pass in the <code>NextToken</code> from the response object of the previous page
     * call.</p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextToken = value; }

    /**
     * <p>A token generated by the Athena service that specifies where to continue
     * pagination if a previous request was truncated. To obtain the next set of pages,
     * pass in the <code>NextToken</code> from the response object of the previous page
     * call.</p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextToken = std::move(value); }

    /**
     * <p>A token generated by the Athena service that specifies where to continue
     * pagination if a previous request was truncated. To obtain the next set of pages,
     * pass in the <code>NextToken</code> from the response object of the previous page
     * call.</p>
     */
    inline void SetNextToken(const char* value) { m_nextToken.assign(value); }

    /**
     * <p>A token generated by the Athena service that specifies where to continue
     * pagination if a previous request was truncated. To obtain the next set of pages,
     * pass in the <code>NextToken</code> from the response object of the previous page
     * call.</p>
     */
    inline ListExecutorsResult& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>A token generated by the Athena service that specifies where to continue
     * pagination if a previous request was truncated. To obtain the next set of pages,
     * pass in the <code>NextToken</code> from the response object of the previous page
     * call.</p>
     */
    inline ListExecutorsResult& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>A token generated by the Athena service that specifies where to continue
     * pagination if a previous request was truncated. To obtain the next set of pages,
     * pass in the <code>NextToken</code> from the response object of the previous page
     * call.</p>
     */
    inline ListExecutorsResult& WithNextToken(const char* value) { SetNextToken(value); return *this;}


    /**
     * <p>Contains summary information about the executor.</p>
     */
    inline const Aws::Vector<ExecutorsSummary>& GetExecutorsSummary() const{ return m_executorsSummary; }

    /**
     * <p>Contains summary information about the executor.</p>
     */
    inline void SetExecutorsSummary(const Aws::Vector<ExecutorsSummary>& value) { m_executorsSummary = value; }

    /**
     * <p>Contains summary information about the executor.</p>
     */
    inline void SetExecutorsSummary(Aws::Vector<ExecutorsSummary>&& value) { m_executorsSummary = std::move(value); }

    /**
     * <p>Contains summary information about the executor.</p>
     */
    inline ListExecutorsResult& WithExecutorsSummary(const Aws::Vector<ExecutorsSummary>& value) { SetExecutorsSummary(value); return *this;}

    /**
     * <p>Contains summary information about the executor.</p>
     */
    inline ListExecutorsResult& WithExecutorsSummary(Aws::Vector<ExecutorsSummary>&& value) { SetExecutorsSummary(std::move(value)); return *this;}

    /**
     * <p>Contains summary information about the executor.</p>
     */
    inline ListExecutorsResult& AddExecutorsSummary(const ExecutorsSummary& value) { m_executorsSummary.push_back(value); return *this; }

    /**
     * <p>Contains summary information about the executor.</p>
     */
    inline ListExecutorsResult& AddExecutorsSummary(ExecutorsSummary&& value) { m_executorsSummary.push_back(std::move(value)); return *this; }

  private:

    Aws::String m_sessionId;

    Aws::String m_nextToken;

    Aws::Vector<ExecutorsSummary> m_executorsSummary;
  };

} // namespace Model
} // namespace Athena
} // namespace Aws
