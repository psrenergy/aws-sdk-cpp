﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/opensearchserverless/OpenSearchServerless_EXPORTS.h>
#include <aws/opensearchserverless/OpenSearchServerlessRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/opensearchserverless/model/AccessPolicyType.h>
#include <utility>

namespace Aws
{
namespace OpenSearchServerless
{
namespace Model
{

  /**
   */
  class AWS_OPENSEARCHSERVERLESS_API ListAccessPoliciesRequest : public OpenSearchServerlessRequest
  {
  public:
    ListAccessPoliciesRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListAccessPolicies"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>An optional parameter that specifies the maximum number of results to return.
     * You can use <code>nextToken</code> to get the next page of results. The default
     * is 20.</p>
     */
    inline int GetMaxResults() const{ return m_maxResults; }

    /**
     * <p>An optional parameter that specifies the maximum number of results to return.
     * You can use <code>nextToken</code> to get the next page of results. The default
     * is 20.</p>
     */
    inline bool MaxResultsHasBeenSet() const { return m_maxResultsHasBeenSet; }

    /**
     * <p>An optional parameter that specifies the maximum number of results to return.
     * You can use <code>nextToken</code> to get the next page of results. The default
     * is 20.</p>
     */
    inline void SetMaxResults(int value) { m_maxResultsHasBeenSet = true; m_maxResults = value; }

    /**
     * <p>An optional parameter that specifies the maximum number of results to return.
     * You can use <code>nextToken</code> to get the next page of results. The default
     * is 20.</p>
     */
    inline ListAccessPoliciesRequest& WithMaxResults(int value) { SetMaxResults(value); return *this;}


    /**
     * <p>If your initial <code>ListAccessPolicies</code> operation returns a
     * <code>nextToken</code>, you can include the returned <code>nextToken</code> in
     * subsequent <code>ListAccessPolicies</code> operations, which returns results in
     * the next page. </p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>If your initial <code>ListAccessPolicies</code> operation returns a
     * <code>nextToken</code>, you can include the returned <code>nextToken</code> in
     * subsequent <code>ListAccessPolicies</code> operations, which returns results in
     * the next page. </p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p>If your initial <code>ListAccessPolicies</code> operation returns a
     * <code>nextToken</code>, you can include the returned <code>nextToken</code> in
     * subsequent <code>ListAccessPolicies</code> operations, which returns results in
     * the next page. </p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p>If your initial <code>ListAccessPolicies</code> operation returns a
     * <code>nextToken</code>, you can include the returned <code>nextToken</code> in
     * subsequent <code>ListAccessPolicies</code> operations, which returns results in
     * the next page. </p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p>If your initial <code>ListAccessPolicies</code> operation returns a
     * <code>nextToken</code>, you can include the returned <code>nextToken</code> in
     * subsequent <code>ListAccessPolicies</code> operations, which returns results in
     * the next page. </p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p>If your initial <code>ListAccessPolicies</code> operation returns a
     * <code>nextToken</code>, you can include the returned <code>nextToken</code> in
     * subsequent <code>ListAccessPolicies</code> operations, which returns results in
     * the next page. </p>
     */
    inline ListAccessPoliciesRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>If your initial <code>ListAccessPolicies</code> operation returns a
     * <code>nextToken</code>, you can include the returned <code>nextToken</code> in
     * subsequent <code>ListAccessPolicies</code> operations, which returns results in
     * the next page. </p>
     */
    inline ListAccessPoliciesRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>If your initial <code>ListAccessPolicies</code> operation returns a
     * <code>nextToken</code>, you can include the returned <code>nextToken</code> in
     * subsequent <code>ListAccessPolicies</code> operations, which returns results in
     * the next page. </p>
     */
    inline ListAccessPoliciesRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}


    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline const Aws::Vector<Aws::String>& GetResource() const{ return m_resource; }

    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline bool ResourceHasBeenSet() const { return m_resourceHasBeenSet; }

    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline void SetResource(const Aws::Vector<Aws::String>& value) { m_resourceHasBeenSet = true; m_resource = value; }

    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline void SetResource(Aws::Vector<Aws::String>&& value) { m_resourceHasBeenSet = true; m_resource = std::move(value); }

    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline ListAccessPoliciesRequest& WithResource(const Aws::Vector<Aws::String>& value) { SetResource(value); return *this;}

    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline ListAccessPoliciesRequest& WithResource(Aws::Vector<Aws::String>&& value) { SetResource(std::move(value)); return *this;}

    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline ListAccessPoliciesRequest& AddResource(const Aws::String& value) { m_resourceHasBeenSet = true; m_resource.push_back(value); return *this; }

    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline ListAccessPoliciesRequest& AddResource(Aws::String&& value) { m_resourceHasBeenSet = true; m_resource.push_back(std::move(value)); return *this; }

    /**
     * <p>Resource filters (can be collection or indexes) that policies can apply
     * to.</p>
     */
    inline ListAccessPoliciesRequest& AddResource(const char* value) { m_resourceHasBeenSet = true; m_resource.push_back(value); return *this; }


    /**
     * <p>The type of access policy.</p>
     */
    inline const AccessPolicyType& GetType() const{ return m_type; }

    /**
     * <p>The type of access policy.</p>
     */
    inline bool TypeHasBeenSet() const { return m_typeHasBeenSet; }

    /**
     * <p>The type of access policy.</p>
     */
    inline void SetType(const AccessPolicyType& value) { m_typeHasBeenSet = true; m_type = value; }

    /**
     * <p>The type of access policy.</p>
     */
    inline void SetType(AccessPolicyType&& value) { m_typeHasBeenSet = true; m_type = std::move(value); }

    /**
     * <p>The type of access policy.</p>
     */
    inline ListAccessPoliciesRequest& WithType(const AccessPolicyType& value) { SetType(value); return *this;}

    /**
     * <p>The type of access policy.</p>
     */
    inline ListAccessPoliciesRequest& WithType(AccessPolicyType&& value) { SetType(std::move(value)); return *this;}

  private:

    int m_maxResults;
    bool m_maxResultsHasBeenSet = false;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;

    Aws::Vector<Aws::String> m_resource;
    bool m_resourceHasBeenSet = false;

    AccessPolicyType m_type;
    bool m_typeHasBeenSet = false;
  };

} // namespace Model
} // namespace OpenSearchServerless
} // namespace Aws
