﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/redshift/Redshift_EXPORTS.h>
#include <aws/redshift/RedshiftRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Redshift
{
namespace Model
{

  /**
   * <p/><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/redshift-2012-12-01/DescribeResizeMessage">AWS
   * API Reference</a></p>
   */
  class AWS_REDSHIFT_API DescribeResizeRequest : public RedshiftRequest
  {
  public:
    DescribeResizeRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeResizeRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeResizeRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeResize"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The unique identifier of a cluster whose resize progress you are requesting.
     * This parameter is case-sensitive.</p> <p>By default, resize operations for all
     * clusters defined for an Amazon Web Services account are returned.</p>
     */
    inline const Aws::String& GetClusterIdentifier() const{ return m_clusterIdentifier; }

    /**
     * <p>The unique identifier of a cluster whose resize progress you are requesting.
     * This parameter is case-sensitive.</p> <p>By default, resize operations for all
     * clusters defined for an Amazon Web Services account are returned.</p>
     */
    inline bool ClusterIdentifierHasBeenSet() const { return m_clusterIdentifierHasBeenSet; }

    /**
     * <p>The unique identifier of a cluster whose resize progress you are requesting.
     * This parameter is case-sensitive.</p> <p>By default, resize operations for all
     * clusters defined for an Amazon Web Services account are returned.</p>
     */
    inline void SetClusterIdentifier(const Aws::String& value) { m_clusterIdentifierHasBeenSet = true; m_clusterIdentifier = value; }

    /**
     * <p>The unique identifier of a cluster whose resize progress you are requesting.
     * This parameter is case-sensitive.</p> <p>By default, resize operations for all
     * clusters defined for an Amazon Web Services account are returned.</p>
     */
    inline void SetClusterIdentifier(Aws::String&& value) { m_clusterIdentifierHasBeenSet = true; m_clusterIdentifier = std::move(value); }

    /**
     * <p>The unique identifier of a cluster whose resize progress you are requesting.
     * This parameter is case-sensitive.</p> <p>By default, resize operations for all
     * clusters defined for an Amazon Web Services account are returned.</p>
     */
    inline void SetClusterIdentifier(const char* value) { m_clusterIdentifierHasBeenSet = true; m_clusterIdentifier.assign(value); }

    /**
     * <p>The unique identifier of a cluster whose resize progress you are requesting.
     * This parameter is case-sensitive.</p> <p>By default, resize operations for all
     * clusters defined for an Amazon Web Services account are returned.</p>
     */
    inline DescribeResizeRequest& WithClusterIdentifier(const Aws::String& value) { SetClusterIdentifier(value); return *this;}

    /**
     * <p>The unique identifier of a cluster whose resize progress you are requesting.
     * This parameter is case-sensitive.</p> <p>By default, resize operations for all
     * clusters defined for an Amazon Web Services account are returned.</p>
     */
    inline DescribeResizeRequest& WithClusterIdentifier(Aws::String&& value) { SetClusterIdentifier(std::move(value)); return *this;}

    /**
     * <p>The unique identifier of a cluster whose resize progress you are requesting.
     * This parameter is case-sensitive.</p> <p>By default, resize operations for all
     * clusters defined for an Amazon Web Services account are returned.</p>
     */
    inline DescribeResizeRequest& WithClusterIdentifier(const char* value) { SetClusterIdentifier(value); return *this;}

  private:

    Aws::String m_clusterIdentifier;
    bool m_clusterIdentifierHasBeenSet = false;
  };

} // namespace Model
} // namespace Redshift
} // namespace Aws
