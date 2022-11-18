﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/events/CloudWatchEvents_EXPORTS.h>
#include <aws/events/CloudWatchEventsRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CloudWatchEvents
{
namespace Model
{

  /**
   */
  class AWS_CLOUDWATCHEVENTS_API ListEventBusesRequest : public CloudWatchEventsRequest
  {
  public:
    ListEventBusesRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<ListEventBusesRequest> Clone() const
    {
      return Aws::MakeUnique<ListEventBusesRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListEventBuses"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>Specifying this limits the results to only those event buses with names that
     * start with the specified prefix.</p>
     */
    inline const Aws::String& GetNamePrefix() const{ return m_namePrefix; }

    /**
     * <p>Specifying this limits the results to only those event buses with names that
     * start with the specified prefix.</p>
     */
    inline bool NamePrefixHasBeenSet() const { return m_namePrefixHasBeenSet; }

    /**
     * <p>Specifying this limits the results to only those event buses with names that
     * start with the specified prefix.</p>
     */
    inline void SetNamePrefix(const Aws::String& value) { m_namePrefixHasBeenSet = true; m_namePrefix = value; }

    /**
     * <p>Specifying this limits the results to only those event buses with names that
     * start with the specified prefix.</p>
     */
    inline void SetNamePrefix(Aws::String&& value) { m_namePrefixHasBeenSet = true; m_namePrefix = std::move(value); }

    /**
     * <p>Specifying this limits the results to only those event buses with names that
     * start with the specified prefix.</p>
     */
    inline void SetNamePrefix(const char* value) { m_namePrefixHasBeenSet = true; m_namePrefix.assign(value); }

    /**
     * <p>Specifying this limits the results to only those event buses with names that
     * start with the specified prefix.</p>
     */
    inline ListEventBusesRequest& WithNamePrefix(const Aws::String& value) { SetNamePrefix(value); return *this;}

    /**
     * <p>Specifying this limits the results to only those event buses with names that
     * start with the specified prefix.</p>
     */
    inline ListEventBusesRequest& WithNamePrefix(Aws::String&& value) { SetNamePrefix(std::move(value)); return *this;}

    /**
     * <p>Specifying this limits the results to only those event buses with names that
     * start with the specified prefix.</p>
     */
    inline ListEventBusesRequest& WithNamePrefix(const char* value) { SetNamePrefix(value); return *this;}


    /**
     * <p>The token returned by a previous call to retrieve the next set of
     * results.</p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>The token returned by a previous call to retrieve the next set of
     * results.</p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p>The token returned by a previous call to retrieve the next set of
     * results.</p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p>The token returned by a previous call to retrieve the next set of
     * results.</p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p>The token returned by a previous call to retrieve the next set of
     * results.</p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p>The token returned by a previous call to retrieve the next set of
     * results.</p>
     */
    inline ListEventBusesRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>The token returned by a previous call to retrieve the next set of
     * results.</p>
     */
    inline ListEventBusesRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>The token returned by a previous call to retrieve the next set of
     * results.</p>
     */
    inline ListEventBusesRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}


    /**
     * <p>Specifying this limits the number of results returned by this operation. The
     * operation also returns a NextToken which you can use in a subsequent operation
     * to retrieve the next set of results.</p>
     */
    inline int GetLimit() const{ return m_limit; }

    /**
     * <p>Specifying this limits the number of results returned by this operation. The
     * operation also returns a NextToken which you can use in a subsequent operation
     * to retrieve the next set of results.</p>
     */
    inline bool LimitHasBeenSet() const { return m_limitHasBeenSet; }

    /**
     * <p>Specifying this limits the number of results returned by this operation. The
     * operation also returns a NextToken which you can use in a subsequent operation
     * to retrieve the next set of results.</p>
     */
    inline void SetLimit(int value) { m_limitHasBeenSet = true; m_limit = value; }

    /**
     * <p>Specifying this limits the number of results returned by this operation. The
     * operation also returns a NextToken which you can use in a subsequent operation
     * to retrieve the next set of results.</p>
     */
    inline ListEventBusesRequest& WithLimit(int value) { SetLimit(value); return *this;}

  private:

    Aws::String m_namePrefix;
    bool m_namePrefixHasBeenSet = false;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;

    int m_limit;
    bool m_limitHasBeenSet = false;
  };

} // namespace Model
} // namespace CloudWatchEvents
} // namespace Aws
