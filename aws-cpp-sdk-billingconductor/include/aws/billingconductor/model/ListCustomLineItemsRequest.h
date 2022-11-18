﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/billingconductor/BillingConductor_EXPORTS.h>
#include <aws/billingconductor/BillingConductorRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/billingconductor/model/ListCustomLineItemsFilter.h>
#include <utility>

namespace Aws
{
namespace BillingConductor
{
namespace Model
{

  /**
   */
  class AWS_BILLINGCONDUCTOR_API ListCustomLineItemsRequest : public BillingConductorRequest
  {
  public:
    ListCustomLineItemsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<ListCustomLineItemsRequest> Clone() const
    {
      return Aws::MakeUnique<ListCustomLineItemsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListCustomLineItems"; }

    Aws::String SerializePayload() const override;


    /**
     * <p> The preferred billing period to get custom line items (FFLIs). </p>
     */
    inline const Aws::String& GetBillingPeriod() const{ return m_billingPeriod; }

    /**
     * <p> The preferred billing period to get custom line items (FFLIs). </p>
     */
    inline bool BillingPeriodHasBeenSet() const { return m_billingPeriodHasBeenSet; }

    /**
     * <p> The preferred billing period to get custom line items (FFLIs). </p>
     */
    inline void SetBillingPeriod(const Aws::String& value) { m_billingPeriodHasBeenSet = true; m_billingPeriod = value; }

    /**
     * <p> The preferred billing period to get custom line items (FFLIs). </p>
     */
    inline void SetBillingPeriod(Aws::String&& value) { m_billingPeriodHasBeenSet = true; m_billingPeriod = std::move(value); }

    /**
     * <p> The preferred billing period to get custom line items (FFLIs). </p>
     */
    inline void SetBillingPeriod(const char* value) { m_billingPeriodHasBeenSet = true; m_billingPeriod.assign(value); }

    /**
     * <p> The preferred billing period to get custom line items (FFLIs). </p>
     */
    inline ListCustomLineItemsRequest& WithBillingPeriod(const Aws::String& value) { SetBillingPeriod(value); return *this;}

    /**
     * <p> The preferred billing period to get custom line items (FFLIs). </p>
     */
    inline ListCustomLineItemsRequest& WithBillingPeriod(Aws::String&& value) { SetBillingPeriod(std::move(value)); return *this;}

    /**
     * <p> The preferred billing period to get custom line items (FFLIs). </p>
     */
    inline ListCustomLineItemsRequest& WithBillingPeriod(const char* value) { SetBillingPeriod(value); return *this;}


    /**
     * <p> The maximum number of billing groups to retrieve. </p>
     */
    inline int GetMaxResults() const{ return m_maxResults; }

    /**
     * <p> The maximum number of billing groups to retrieve. </p>
     */
    inline bool MaxResultsHasBeenSet() const { return m_maxResultsHasBeenSet; }

    /**
     * <p> The maximum number of billing groups to retrieve. </p>
     */
    inline void SetMaxResults(int value) { m_maxResultsHasBeenSet = true; m_maxResults = value; }

    /**
     * <p> The maximum number of billing groups to retrieve. </p>
     */
    inline ListCustomLineItemsRequest& WithMaxResults(int value) { SetMaxResults(value); return *this;}


    /**
     * <p> The pagination token that's used on subsequent calls to get custom line
     * items (FFLIs). </p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p> The pagination token that's used on subsequent calls to get custom line
     * items (FFLIs). </p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p> The pagination token that's used on subsequent calls to get custom line
     * items (FFLIs). </p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p> The pagination token that's used on subsequent calls to get custom line
     * items (FFLIs). </p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p> The pagination token that's used on subsequent calls to get custom line
     * items (FFLIs). </p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p> The pagination token that's used on subsequent calls to get custom line
     * items (FFLIs). </p>
     */
    inline ListCustomLineItemsRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p> The pagination token that's used on subsequent calls to get custom line
     * items (FFLIs). </p>
     */
    inline ListCustomLineItemsRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p> The pagination token that's used on subsequent calls to get custom line
     * items (FFLIs). </p>
     */
    inline ListCustomLineItemsRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}


    /**
     * <p>A <code>ListCustomLineItemsFilter</code> that specifies the custom line item
     * names and/or billing group Amazon Resource Names (ARNs) to retrieve FFLI
     * information.</p>
     */
    inline const ListCustomLineItemsFilter& GetFilters() const{ return m_filters; }

    /**
     * <p>A <code>ListCustomLineItemsFilter</code> that specifies the custom line item
     * names and/or billing group Amazon Resource Names (ARNs) to retrieve FFLI
     * information.</p>
     */
    inline bool FiltersHasBeenSet() const { return m_filtersHasBeenSet; }

    /**
     * <p>A <code>ListCustomLineItemsFilter</code> that specifies the custom line item
     * names and/or billing group Amazon Resource Names (ARNs) to retrieve FFLI
     * information.</p>
     */
    inline void SetFilters(const ListCustomLineItemsFilter& value) { m_filtersHasBeenSet = true; m_filters = value; }

    /**
     * <p>A <code>ListCustomLineItemsFilter</code> that specifies the custom line item
     * names and/or billing group Amazon Resource Names (ARNs) to retrieve FFLI
     * information.</p>
     */
    inline void SetFilters(ListCustomLineItemsFilter&& value) { m_filtersHasBeenSet = true; m_filters = std::move(value); }

    /**
     * <p>A <code>ListCustomLineItemsFilter</code> that specifies the custom line item
     * names and/or billing group Amazon Resource Names (ARNs) to retrieve FFLI
     * information.</p>
     */
    inline ListCustomLineItemsRequest& WithFilters(const ListCustomLineItemsFilter& value) { SetFilters(value); return *this;}

    /**
     * <p>A <code>ListCustomLineItemsFilter</code> that specifies the custom line item
     * names and/or billing group Amazon Resource Names (ARNs) to retrieve FFLI
     * information.</p>
     */
    inline ListCustomLineItemsRequest& WithFilters(ListCustomLineItemsFilter&& value) { SetFilters(std::move(value)); return *this;}

  private:

    Aws::String m_billingPeriod;
    bool m_billingPeriodHasBeenSet = false;

    int m_maxResults;
    bool m_maxResultsHasBeenSet = false;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;

    ListCustomLineItemsFilter m_filters;
    bool m_filtersHasBeenSet = false;
  };

} // namespace Model
} // namespace BillingConductor
} // namespace Aws
