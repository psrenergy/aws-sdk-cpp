﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/rds/RDS_EXPORTS.h>
#include <aws/rds/RDSRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace RDS
{
namespace Model
{

  /**
   * <p/><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/rds-2014-10-31/AddSourceIdentifierToSubscriptionMessage">AWS
   * API Reference</a></p>
   */
  class AWS_RDS_API AddSourceIdentifierToSubscriptionRequest : public RDSRequest
  {
  public:
    AddSourceIdentifierToSubscriptionRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "AddSourceIdentifierToSubscription"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The name of the RDS event notification subscription you want to add a source
     * identifier to.</p>
     */
    inline const Aws::String& GetSubscriptionName() const{ return m_subscriptionName; }

    /**
     * <p>The name of the RDS event notification subscription you want to add a source
     * identifier to.</p>
     */
    inline bool SubscriptionNameHasBeenSet() const { return m_subscriptionNameHasBeenSet; }

    /**
     * <p>The name of the RDS event notification subscription you want to add a source
     * identifier to.</p>
     */
    inline void SetSubscriptionName(const Aws::String& value) { m_subscriptionNameHasBeenSet = true; m_subscriptionName = value; }

    /**
     * <p>The name of the RDS event notification subscription you want to add a source
     * identifier to.</p>
     */
    inline void SetSubscriptionName(Aws::String&& value) { m_subscriptionNameHasBeenSet = true; m_subscriptionName = std::move(value); }

    /**
     * <p>The name of the RDS event notification subscription you want to add a source
     * identifier to.</p>
     */
    inline void SetSubscriptionName(const char* value) { m_subscriptionNameHasBeenSet = true; m_subscriptionName.assign(value); }

    /**
     * <p>The name of the RDS event notification subscription you want to add a source
     * identifier to.</p>
     */
    inline AddSourceIdentifierToSubscriptionRequest& WithSubscriptionName(const Aws::String& value) { SetSubscriptionName(value); return *this;}

    /**
     * <p>The name of the RDS event notification subscription you want to add a source
     * identifier to.</p>
     */
    inline AddSourceIdentifierToSubscriptionRequest& WithSubscriptionName(Aws::String&& value) { SetSubscriptionName(std::move(value)); return *this;}

    /**
     * <p>The name of the RDS event notification subscription you want to add a source
     * identifier to.</p>
     */
    inline AddSourceIdentifierToSubscriptionRequest& WithSubscriptionName(const char* value) { SetSubscriptionName(value); return *this;}


    /**
     * <p>The identifier of the event source to be added.</p> <p>Constraints:</p> <ul>
     * <li> <p>If the source type is a DB instance, a <code>DBInstanceIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster, a
     * <code>DBClusterIdentifier</code> value must be supplied.</p> </li> <li> <p>If
     * the source type is a DB parameter group, a <code>DBParameterGroupName</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB security
     * group, a <code>DBSecurityGroupName</code> value must be supplied.</p> </li> <li>
     * <p>If the source type is a DB snapshot, a <code>DBSnapshotIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster
     * snapshot, a <code>DBClusterSnapshotIdentifier</code> value must be supplied.</p>
     * </li> <li> <p>If the source type is an RDS Proxy, a <code>DBProxyName</code>
     * value must be supplied.</p> </li> </ul>
     */
    inline const Aws::String& GetSourceIdentifier() const{ return m_sourceIdentifier; }

    /**
     * <p>The identifier of the event source to be added.</p> <p>Constraints:</p> <ul>
     * <li> <p>If the source type is a DB instance, a <code>DBInstanceIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster, a
     * <code>DBClusterIdentifier</code> value must be supplied.</p> </li> <li> <p>If
     * the source type is a DB parameter group, a <code>DBParameterGroupName</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB security
     * group, a <code>DBSecurityGroupName</code> value must be supplied.</p> </li> <li>
     * <p>If the source type is a DB snapshot, a <code>DBSnapshotIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster
     * snapshot, a <code>DBClusterSnapshotIdentifier</code> value must be supplied.</p>
     * </li> <li> <p>If the source type is an RDS Proxy, a <code>DBProxyName</code>
     * value must be supplied.</p> </li> </ul>
     */
    inline bool SourceIdentifierHasBeenSet() const { return m_sourceIdentifierHasBeenSet; }

    /**
     * <p>The identifier of the event source to be added.</p> <p>Constraints:</p> <ul>
     * <li> <p>If the source type is a DB instance, a <code>DBInstanceIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster, a
     * <code>DBClusterIdentifier</code> value must be supplied.</p> </li> <li> <p>If
     * the source type is a DB parameter group, a <code>DBParameterGroupName</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB security
     * group, a <code>DBSecurityGroupName</code> value must be supplied.</p> </li> <li>
     * <p>If the source type is a DB snapshot, a <code>DBSnapshotIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster
     * snapshot, a <code>DBClusterSnapshotIdentifier</code> value must be supplied.</p>
     * </li> <li> <p>If the source type is an RDS Proxy, a <code>DBProxyName</code>
     * value must be supplied.</p> </li> </ul>
     */
    inline void SetSourceIdentifier(const Aws::String& value) { m_sourceIdentifierHasBeenSet = true; m_sourceIdentifier = value; }

    /**
     * <p>The identifier of the event source to be added.</p> <p>Constraints:</p> <ul>
     * <li> <p>If the source type is a DB instance, a <code>DBInstanceIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster, a
     * <code>DBClusterIdentifier</code> value must be supplied.</p> </li> <li> <p>If
     * the source type is a DB parameter group, a <code>DBParameterGroupName</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB security
     * group, a <code>DBSecurityGroupName</code> value must be supplied.</p> </li> <li>
     * <p>If the source type is a DB snapshot, a <code>DBSnapshotIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster
     * snapshot, a <code>DBClusterSnapshotIdentifier</code> value must be supplied.</p>
     * </li> <li> <p>If the source type is an RDS Proxy, a <code>DBProxyName</code>
     * value must be supplied.</p> </li> </ul>
     */
    inline void SetSourceIdentifier(Aws::String&& value) { m_sourceIdentifierHasBeenSet = true; m_sourceIdentifier = std::move(value); }

    /**
     * <p>The identifier of the event source to be added.</p> <p>Constraints:</p> <ul>
     * <li> <p>If the source type is a DB instance, a <code>DBInstanceIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster, a
     * <code>DBClusterIdentifier</code> value must be supplied.</p> </li> <li> <p>If
     * the source type is a DB parameter group, a <code>DBParameterGroupName</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB security
     * group, a <code>DBSecurityGroupName</code> value must be supplied.</p> </li> <li>
     * <p>If the source type is a DB snapshot, a <code>DBSnapshotIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster
     * snapshot, a <code>DBClusterSnapshotIdentifier</code> value must be supplied.</p>
     * </li> <li> <p>If the source type is an RDS Proxy, a <code>DBProxyName</code>
     * value must be supplied.</p> </li> </ul>
     */
    inline void SetSourceIdentifier(const char* value) { m_sourceIdentifierHasBeenSet = true; m_sourceIdentifier.assign(value); }

    /**
     * <p>The identifier of the event source to be added.</p> <p>Constraints:</p> <ul>
     * <li> <p>If the source type is a DB instance, a <code>DBInstanceIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster, a
     * <code>DBClusterIdentifier</code> value must be supplied.</p> </li> <li> <p>If
     * the source type is a DB parameter group, a <code>DBParameterGroupName</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB security
     * group, a <code>DBSecurityGroupName</code> value must be supplied.</p> </li> <li>
     * <p>If the source type is a DB snapshot, a <code>DBSnapshotIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster
     * snapshot, a <code>DBClusterSnapshotIdentifier</code> value must be supplied.</p>
     * </li> <li> <p>If the source type is an RDS Proxy, a <code>DBProxyName</code>
     * value must be supplied.</p> </li> </ul>
     */
    inline AddSourceIdentifierToSubscriptionRequest& WithSourceIdentifier(const Aws::String& value) { SetSourceIdentifier(value); return *this;}

    /**
     * <p>The identifier of the event source to be added.</p> <p>Constraints:</p> <ul>
     * <li> <p>If the source type is a DB instance, a <code>DBInstanceIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster, a
     * <code>DBClusterIdentifier</code> value must be supplied.</p> </li> <li> <p>If
     * the source type is a DB parameter group, a <code>DBParameterGroupName</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB security
     * group, a <code>DBSecurityGroupName</code> value must be supplied.</p> </li> <li>
     * <p>If the source type is a DB snapshot, a <code>DBSnapshotIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster
     * snapshot, a <code>DBClusterSnapshotIdentifier</code> value must be supplied.</p>
     * </li> <li> <p>If the source type is an RDS Proxy, a <code>DBProxyName</code>
     * value must be supplied.</p> </li> </ul>
     */
    inline AddSourceIdentifierToSubscriptionRequest& WithSourceIdentifier(Aws::String&& value) { SetSourceIdentifier(std::move(value)); return *this;}

    /**
     * <p>The identifier of the event source to be added.</p> <p>Constraints:</p> <ul>
     * <li> <p>If the source type is a DB instance, a <code>DBInstanceIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster, a
     * <code>DBClusterIdentifier</code> value must be supplied.</p> </li> <li> <p>If
     * the source type is a DB parameter group, a <code>DBParameterGroupName</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB security
     * group, a <code>DBSecurityGroupName</code> value must be supplied.</p> </li> <li>
     * <p>If the source type is a DB snapshot, a <code>DBSnapshotIdentifier</code>
     * value must be supplied.</p> </li> <li> <p>If the source type is a DB cluster
     * snapshot, a <code>DBClusterSnapshotIdentifier</code> value must be supplied.</p>
     * </li> <li> <p>If the source type is an RDS Proxy, a <code>DBProxyName</code>
     * value must be supplied.</p> </li> </ul>
     */
    inline AddSourceIdentifierToSubscriptionRequest& WithSourceIdentifier(const char* value) { SetSourceIdentifier(value); return *this;}

  private:

    Aws::String m_subscriptionName;
    bool m_subscriptionNameHasBeenSet = false;

    Aws::String m_sourceIdentifier;
    bool m_sourceIdentifierHasBeenSet = false;
  };

} // namespace Model
} // namespace RDS
} // namespace Aws
