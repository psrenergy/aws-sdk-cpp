﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/apprunner/AppRunner_EXPORTS.h>
#include <aws/apprunner/AppRunnerRequest.h>
#include <aws/apprunner/model/ListVpcIngressConnectionsFilter.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace AppRunner
{
namespace Model
{

  /**
   */
  class AWS_APPRUNNER_API ListVpcIngressConnectionsRequest : public AppRunnerRequest
  {
  public:
    ListVpcIngressConnectionsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<ListVpcIngressConnectionsRequest> Clone() const
    {
      return Aws::MakeUnique<ListVpcIngressConnectionsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListVpcIngressConnections"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The VPC Ingress Connections to be listed based on either the Service Arn or
     * Vpc Endpoint Id, or both.</p>
     */
    inline const ListVpcIngressConnectionsFilter& GetFilter() const{ return m_filter; }

    /**
     * <p>The VPC Ingress Connections to be listed based on either the Service Arn or
     * Vpc Endpoint Id, or both.</p>
     */
    inline bool FilterHasBeenSet() const { return m_filterHasBeenSet; }

    /**
     * <p>The VPC Ingress Connections to be listed based on either the Service Arn or
     * Vpc Endpoint Id, or both.</p>
     */
    inline void SetFilter(const ListVpcIngressConnectionsFilter& value) { m_filterHasBeenSet = true; m_filter = value; }

    /**
     * <p>The VPC Ingress Connections to be listed based on either the Service Arn or
     * Vpc Endpoint Id, or both.</p>
     */
    inline void SetFilter(ListVpcIngressConnectionsFilter&& value) { m_filterHasBeenSet = true; m_filter = std::move(value); }

    /**
     * <p>The VPC Ingress Connections to be listed based on either the Service Arn or
     * Vpc Endpoint Id, or both.</p>
     */
    inline ListVpcIngressConnectionsRequest& WithFilter(const ListVpcIngressConnectionsFilter& value) { SetFilter(value); return *this;}

    /**
     * <p>The VPC Ingress Connections to be listed based on either the Service Arn or
     * Vpc Endpoint Id, or both.</p>
     */
    inline ListVpcIngressConnectionsRequest& WithFilter(ListVpcIngressConnectionsFilter&& value) { SetFilter(std::move(value)); return *this;}


    /**
     * <p>The maximum number of results to include in each response (result page). It's
     * used for a paginated request.</p> <p>If you don't specify
     * <code>MaxResults</code>, the request retrieves all available results in a single
     * response.</p>
     */
    inline int GetMaxResults() const{ return m_maxResults; }

    /**
     * <p>The maximum number of results to include in each response (result page). It's
     * used for a paginated request.</p> <p>If you don't specify
     * <code>MaxResults</code>, the request retrieves all available results in a single
     * response.</p>
     */
    inline bool MaxResultsHasBeenSet() const { return m_maxResultsHasBeenSet; }

    /**
     * <p>The maximum number of results to include in each response (result page). It's
     * used for a paginated request.</p> <p>If you don't specify
     * <code>MaxResults</code>, the request retrieves all available results in a single
     * response.</p>
     */
    inline void SetMaxResults(int value) { m_maxResultsHasBeenSet = true; m_maxResults = value; }

    /**
     * <p>The maximum number of results to include in each response (result page). It's
     * used for a paginated request.</p> <p>If you don't specify
     * <code>MaxResults</code>, the request retrieves all available results in a single
     * response.</p>
     */
    inline ListVpcIngressConnectionsRequest& WithMaxResults(int value) { SetMaxResults(value); return *this;}


    /**
     * <p>A token from a previous result page. It's used for a paginated request. The
     * request retrieves the next result page. All other parameter values must be
     * identical to the ones that are specified in the initial request.</p> <p>If you
     * don't specify <code>NextToken</code>, the request retrieves the first result
     * page.</p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>A token from a previous result page. It's used for a paginated request. The
     * request retrieves the next result page. All other parameter values must be
     * identical to the ones that are specified in the initial request.</p> <p>If you
     * don't specify <code>NextToken</code>, the request retrieves the first result
     * page.</p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p>A token from a previous result page. It's used for a paginated request. The
     * request retrieves the next result page. All other parameter values must be
     * identical to the ones that are specified in the initial request.</p> <p>If you
     * don't specify <code>NextToken</code>, the request retrieves the first result
     * page.</p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p>A token from a previous result page. It's used for a paginated request. The
     * request retrieves the next result page. All other parameter values must be
     * identical to the ones that are specified in the initial request.</p> <p>If you
     * don't specify <code>NextToken</code>, the request retrieves the first result
     * page.</p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p>A token from a previous result page. It's used for a paginated request. The
     * request retrieves the next result page. All other parameter values must be
     * identical to the ones that are specified in the initial request.</p> <p>If you
     * don't specify <code>NextToken</code>, the request retrieves the first result
     * page.</p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p>A token from a previous result page. It's used for a paginated request. The
     * request retrieves the next result page. All other parameter values must be
     * identical to the ones that are specified in the initial request.</p> <p>If you
     * don't specify <code>NextToken</code>, the request retrieves the first result
     * page.</p>
     */
    inline ListVpcIngressConnectionsRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>A token from a previous result page. It's used for a paginated request. The
     * request retrieves the next result page. All other parameter values must be
     * identical to the ones that are specified in the initial request.</p> <p>If you
     * don't specify <code>NextToken</code>, the request retrieves the first result
     * page.</p>
     */
    inline ListVpcIngressConnectionsRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>A token from a previous result page. It's used for a paginated request. The
     * request retrieves the next result page. All other parameter values must be
     * identical to the ones that are specified in the initial request.</p> <p>If you
     * don't specify <code>NextToken</code>, the request retrieves the first result
     * page.</p>
     */
    inline ListVpcIngressConnectionsRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}

  private:

    ListVpcIngressConnectionsFilter m_filter;
    bool m_filterHasBeenSet = false;

    int m_maxResults;
    bool m_maxResultsHasBeenSet = false;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;
  };

} // namespace Model
} // namespace AppRunner
} // namespace Aws
