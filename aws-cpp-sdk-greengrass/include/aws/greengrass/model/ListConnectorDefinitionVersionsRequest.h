﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/greengrass/Greengrass_EXPORTS.h>
#include <aws/greengrass/GreengrassRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Http
{
    class URI;
} //namespace Http
namespace Greengrass
{
namespace Model
{

  /**
   */
  class AWS_GREENGRASS_API ListConnectorDefinitionVersionsRequest : public GreengrassRequest
  {
  public:
    ListConnectorDefinitionVersionsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<ListConnectorDefinitionVersionsRequest> Clone() const
    {
      return Aws::MakeUnique<ListConnectorDefinitionVersionsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListConnectorDefinitionVersions"; }

    Aws::String SerializePayload() const override;

    void AddQueryStringParameters(Aws::Http::URI& uri) const override;


    /**
     * The ID of the connector definition.
     */
    inline const Aws::String& GetConnectorDefinitionId() const{ return m_connectorDefinitionId; }

    /**
     * The ID of the connector definition.
     */
    inline bool ConnectorDefinitionIdHasBeenSet() const { return m_connectorDefinitionIdHasBeenSet; }

    /**
     * The ID of the connector definition.
     */
    inline void SetConnectorDefinitionId(const Aws::String& value) { m_connectorDefinitionIdHasBeenSet = true; m_connectorDefinitionId = value; }

    /**
     * The ID of the connector definition.
     */
    inline void SetConnectorDefinitionId(Aws::String&& value) { m_connectorDefinitionIdHasBeenSet = true; m_connectorDefinitionId = std::move(value); }

    /**
     * The ID of the connector definition.
     */
    inline void SetConnectorDefinitionId(const char* value) { m_connectorDefinitionIdHasBeenSet = true; m_connectorDefinitionId.assign(value); }

    /**
     * The ID of the connector definition.
     */
    inline ListConnectorDefinitionVersionsRequest& WithConnectorDefinitionId(const Aws::String& value) { SetConnectorDefinitionId(value); return *this;}

    /**
     * The ID of the connector definition.
     */
    inline ListConnectorDefinitionVersionsRequest& WithConnectorDefinitionId(Aws::String&& value) { SetConnectorDefinitionId(std::move(value)); return *this;}

    /**
     * The ID of the connector definition.
     */
    inline ListConnectorDefinitionVersionsRequest& WithConnectorDefinitionId(const char* value) { SetConnectorDefinitionId(value); return *this;}


    /**
     * The maximum number of results to be returned per request.
     */
    inline const Aws::String& GetMaxResults() const{ return m_maxResults; }

    /**
     * The maximum number of results to be returned per request.
     */
    inline bool MaxResultsHasBeenSet() const { return m_maxResultsHasBeenSet; }

    /**
     * The maximum number of results to be returned per request.
     */
    inline void SetMaxResults(const Aws::String& value) { m_maxResultsHasBeenSet = true; m_maxResults = value; }

    /**
     * The maximum number of results to be returned per request.
     */
    inline void SetMaxResults(Aws::String&& value) { m_maxResultsHasBeenSet = true; m_maxResults = std::move(value); }

    /**
     * The maximum number of results to be returned per request.
     */
    inline void SetMaxResults(const char* value) { m_maxResultsHasBeenSet = true; m_maxResults.assign(value); }

    /**
     * The maximum number of results to be returned per request.
     */
    inline ListConnectorDefinitionVersionsRequest& WithMaxResults(const Aws::String& value) { SetMaxResults(value); return *this;}

    /**
     * The maximum number of results to be returned per request.
     */
    inline ListConnectorDefinitionVersionsRequest& WithMaxResults(Aws::String&& value) { SetMaxResults(std::move(value)); return *this;}

    /**
     * The maximum number of results to be returned per request.
     */
    inline ListConnectorDefinitionVersionsRequest& WithMaxResults(const char* value) { SetMaxResults(value); return *this;}


    /**
     * The token for the next set of results, or ''null'' if there are no additional
     * results.
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * The token for the next set of results, or ''null'' if there are no additional
     * results.
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * The token for the next set of results, or ''null'' if there are no additional
     * results.
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * The token for the next set of results, or ''null'' if there are no additional
     * results.
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * The token for the next set of results, or ''null'' if there are no additional
     * results.
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * The token for the next set of results, or ''null'' if there are no additional
     * results.
     */
    inline ListConnectorDefinitionVersionsRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * The token for the next set of results, or ''null'' if there are no additional
     * results.
     */
    inline ListConnectorDefinitionVersionsRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * The token for the next set of results, or ''null'' if there are no additional
     * results.
     */
    inline ListConnectorDefinitionVersionsRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}

  private:

    Aws::String m_connectorDefinitionId;
    bool m_connectorDefinitionIdHasBeenSet = false;

    Aws::String m_maxResults;
    bool m_maxResultsHasBeenSet = false;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;
  };

} // namespace Model
} // namespace Greengrass
} // namespace Aws
