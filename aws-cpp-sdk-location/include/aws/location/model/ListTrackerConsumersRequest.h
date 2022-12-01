﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/location/LocationService_EXPORTS.h>
#include <aws/location/LocationServiceRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace LocationService
{
namespace Model
{

  /**
   */
  class AWS_LOCATIONSERVICE_API ListTrackerConsumersRequest : public LocationServiceRequest
  {
  public:
    ListTrackerConsumersRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<ListTrackerConsumersRequest> Clone() const
    {
      return Aws::MakeUnique<ListTrackerConsumersRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListTrackerConsumers"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>An optional limit for the number of resources returned in a single call. </p>
     * <p>Default value: <code>100</code> </p>
     */
    inline int GetMaxResults() const{ return m_maxResults; }

    /**
     * <p>An optional limit for the number of resources returned in a single call. </p>
     * <p>Default value: <code>100</code> </p>
     */
    inline bool MaxResultsHasBeenSet() const { return m_maxResultsHasBeenSet; }

    /**
     * <p>An optional limit for the number of resources returned in a single call. </p>
     * <p>Default value: <code>100</code> </p>
     */
    inline void SetMaxResults(int value) { m_maxResultsHasBeenSet = true; m_maxResults = value; }

    /**
     * <p>An optional limit for the number of resources returned in a single call. </p>
     * <p>Default value: <code>100</code> </p>
     */
    inline ListTrackerConsumersRequest& WithMaxResults(int value) { SetMaxResults(value); return *this;}


    /**
     * <p>The pagination token specifying which page of results to return in the
     * response. If no token is provided, the default page is the first page. </p>
     * <p>Default value: <code>null</code> </p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>The pagination token specifying which page of results to return in the
     * response. If no token is provided, the default page is the first page. </p>
     * <p>Default value: <code>null</code> </p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p>The pagination token specifying which page of results to return in the
     * response. If no token is provided, the default page is the first page. </p>
     * <p>Default value: <code>null</code> </p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p>The pagination token specifying which page of results to return in the
     * response. If no token is provided, the default page is the first page. </p>
     * <p>Default value: <code>null</code> </p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p>The pagination token specifying which page of results to return in the
     * response. If no token is provided, the default page is the first page. </p>
     * <p>Default value: <code>null</code> </p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p>The pagination token specifying which page of results to return in the
     * response. If no token is provided, the default page is the first page. </p>
     * <p>Default value: <code>null</code> </p>
     */
    inline ListTrackerConsumersRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>The pagination token specifying which page of results to return in the
     * response. If no token is provided, the default page is the first page. </p>
     * <p>Default value: <code>null</code> </p>
     */
    inline ListTrackerConsumersRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>The pagination token specifying which page of results to return in the
     * response. If no token is provided, the default page is the first page. </p>
     * <p>Default value: <code>null</code> </p>
     */
    inline ListTrackerConsumersRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}


    /**
     * <p>The tracker resource whose associated geofence collections you want to
     * list.</p>
     */
    inline const Aws::String& GetTrackerName() const{ return m_trackerName; }

    /**
     * <p>The tracker resource whose associated geofence collections you want to
     * list.</p>
     */
    inline bool TrackerNameHasBeenSet() const { return m_trackerNameHasBeenSet; }

    /**
     * <p>The tracker resource whose associated geofence collections you want to
     * list.</p>
     */
    inline void SetTrackerName(const Aws::String& value) { m_trackerNameHasBeenSet = true; m_trackerName = value; }

    /**
     * <p>The tracker resource whose associated geofence collections you want to
     * list.</p>
     */
    inline void SetTrackerName(Aws::String&& value) { m_trackerNameHasBeenSet = true; m_trackerName = std::move(value); }

    /**
     * <p>The tracker resource whose associated geofence collections you want to
     * list.</p>
     */
    inline void SetTrackerName(const char* value) { m_trackerNameHasBeenSet = true; m_trackerName.assign(value); }

    /**
     * <p>The tracker resource whose associated geofence collections you want to
     * list.</p>
     */
    inline ListTrackerConsumersRequest& WithTrackerName(const Aws::String& value) { SetTrackerName(value); return *this;}

    /**
     * <p>The tracker resource whose associated geofence collections you want to
     * list.</p>
     */
    inline ListTrackerConsumersRequest& WithTrackerName(Aws::String&& value) { SetTrackerName(std::move(value)); return *this;}

    /**
     * <p>The tracker resource whose associated geofence collections you want to
     * list.</p>
     */
    inline ListTrackerConsumersRequest& WithTrackerName(const char* value) { SetTrackerName(value); return *this;}

  private:

    int m_maxResults;
    bool m_maxResultsHasBeenSet = false;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;

    Aws::String m_trackerName;
    bool m_trackerNameHasBeenSet = false;
  };

} // namespace Model
} // namespace LocationService
} // namespace Aws
