﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/apigatewayv2/ApiGatewayV2_EXPORTS.h>
#include <aws/apigatewayv2/ApiGatewayV2Request.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Http
{
    class URI;
} //namespace Http
namespace ApiGatewayV2
{
namespace Model
{

  /**
   */
  class AWS_APIGATEWAYV2_API GetApiMappingsRequest : public ApiGatewayV2Request
  {
  public:
    GetApiMappingsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetApiMappingsRequest> Clone() const
    {
      return Aws::MakeUnique<GetApiMappingsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetApiMappings"; }

    Aws::String SerializePayload() const override;

    void AddQueryStringParameters(Aws::Http::URI& uri) const override;


    /**
     * <p>The domain name.</p>
     */
    inline const Aws::String& GetDomainName() const{ return m_domainName; }

    /**
     * <p>The domain name.</p>
     */
    inline bool DomainNameHasBeenSet() const { return m_domainNameHasBeenSet; }

    /**
     * <p>The domain name.</p>
     */
    inline void SetDomainName(const Aws::String& value) { m_domainNameHasBeenSet = true; m_domainName = value; }

    /**
     * <p>The domain name.</p>
     */
    inline void SetDomainName(Aws::String&& value) { m_domainNameHasBeenSet = true; m_domainName = std::move(value); }

    /**
     * <p>The domain name.</p>
     */
    inline void SetDomainName(const char* value) { m_domainNameHasBeenSet = true; m_domainName.assign(value); }

    /**
     * <p>The domain name.</p>
     */
    inline GetApiMappingsRequest& WithDomainName(const Aws::String& value) { SetDomainName(value); return *this;}

    /**
     * <p>The domain name.</p>
     */
    inline GetApiMappingsRequest& WithDomainName(Aws::String&& value) { SetDomainName(std::move(value)); return *this;}

    /**
     * <p>The domain name.</p>
     */
    inline GetApiMappingsRequest& WithDomainName(const char* value) { SetDomainName(value); return *this;}


    /**
     * <p>The maximum number of elements to be returned for this resource.</p>
     */
    inline const Aws::String& GetMaxResults() const{ return m_maxResults; }

    /**
     * <p>The maximum number of elements to be returned for this resource.</p>
     */
    inline bool MaxResultsHasBeenSet() const { return m_maxResultsHasBeenSet; }

    /**
     * <p>The maximum number of elements to be returned for this resource.</p>
     */
    inline void SetMaxResults(const Aws::String& value) { m_maxResultsHasBeenSet = true; m_maxResults = value; }

    /**
     * <p>The maximum number of elements to be returned for this resource.</p>
     */
    inline void SetMaxResults(Aws::String&& value) { m_maxResultsHasBeenSet = true; m_maxResults = std::move(value); }

    /**
     * <p>The maximum number of elements to be returned for this resource.</p>
     */
    inline void SetMaxResults(const char* value) { m_maxResultsHasBeenSet = true; m_maxResults.assign(value); }

    /**
     * <p>The maximum number of elements to be returned for this resource.</p>
     */
    inline GetApiMappingsRequest& WithMaxResults(const Aws::String& value) { SetMaxResults(value); return *this;}

    /**
     * <p>The maximum number of elements to be returned for this resource.</p>
     */
    inline GetApiMappingsRequest& WithMaxResults(Aws::String&& value) { SetMaxResults(std::move(value)); return *this;}

    /**
     * <p>The maximum number of elements to be returned for this resource.</p>
     */
    inline GetApiMappingsRequest& WithMaxResults(const char* value) { SetMaxResults(value); return *this;}


    /**
     * <p>The next page of elements from this collection. Not valid for the last
     * element of the collection.</p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>The next page of elements from this collection. Not valid for the last
     * element of the collection.</p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p>The next page of elements from this collection. Not valid for the last
     * element of the collection.</p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p>The next page of elements from this collection. Not valid for the last
     * element of the collection.</p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p>The next page of elements from this collection. Not valid for the last
     * element of the collection.</p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p>The next page of elements from this collection. Not valid for the last
     * element of the collection.</p>
     */
    inline GetApiMappingsRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>The next page of elements from this collection. Not valid for the last
     * element of the collection.</p>
     */
    inline GetApiMappingsRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>The next page of elements from this collection. Not valid for the last
     * element of the collection.</p>
     */
    inline GetApiMappingsRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}

  private:

    Aws::String m_domainName;
    bool m_domainNameHasBeenSet = false;

    Aws::String m_maxResults;
    bool m_maxResultsHasBeenSet = false;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;
  };

} // namespace Model
} // namespace ApiGatewayV2
} // namespace Aws
