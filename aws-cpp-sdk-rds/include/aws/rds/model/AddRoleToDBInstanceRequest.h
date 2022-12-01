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
   */
  class AWS_RDS_API AddRoleToDBInstanceRequest : public RDSRequest
  {
  public:
    AddRoleToDBInstanceRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<AddRoleToDBInstanceRequest> Clone() const
    {
      return Aws::MakeUnique<AddRoleToDBInstanceRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "AddRoleToDBInstance"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The name of the DB instance to associate the IAM role with.</p>
     */
    inline const Aws::String& GetDBInstanceIdentifier() const{ return m_dBInstanceIdentifier; }

    /**
     * <p>The name of the DB instance to associate the IAM role with.</p>
     */
    inline bool DBInstanceIdentifierHasBeenSet() const { return m_dBInstanceIdentifierHasBeenSet; }

    /**
     * <p>The name of the DB instance to associate the IAM role with.</p>
     */
    inline void SetDBInstanceIdentifier(const Aws::String& value) { m_dBInstanceIdentifierHasBeenSet = true; m_dBInstanceIdentifier = value; }

    /**
     * <p>The name of the DB instance to associate the IAM role with.</p>
     */
    inline void SetDBInstanceIdentifier(Aws::String&& value) { m_dBInstanceIdentifierHasBeenSet = true; m_dBInstanceIdentifier = std::move(value); }

    /**
     * <p>The name of the DB instance to associate the IAM role with.</p>
     */
    inline void SetDBInstanceIdentifier(const char* value) { m_dBInstanceIdentifierHasBeenSet = true; m_dBInstanceIdentifier.assign(value); }

    /**
     * <p>The name of the DB instance to associate the IAM role with.</p>
     */
    inline AddRoleToDBInstanceRequest& WithDBInstanceIdentifier(const Aws::String& value) { SetDBInstanceIdentifier(value); return *this;}

    /**
     * <p>The name of the DB instance to associate the IAM role with.</p>
     */
    inline AddRoleToDBInstanceRequest& WithDBInstanceIdentifier(Aws::String&& value) { SetDBInstanceIdentifier(std::move(value)); return *this;}

    /**
     * <p>The name of the DB instance to associate the IAM role with.</p>
     */
    inline AddRoleToDBInstanceRequest& WithDBInstanceIdentifier(const char* value) { SetDBInstanceIdentifier(value); return *this;}


    /**
     * <p>The Amazon Resource Name (ARN) of the IAM role to associate with the DB
     * instance, for example
     * <code>arn:aws:iam::123456789012:role/AccessRole</code>.</p>
     */
    inline const Aws::String& GetRoleArn() const{ return m_roleArn; }

    /**
     * <p>The Amazon Resource Name (ARN) of the IAM role to associate with the DB
     * instance, for example
     * <code>arn:aws:iam::123456789012:role/AccessRole</code>.</p>
     */
    inline bool RoleArnHasBeenSet() const { return m_roleArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) of the IAM role to associate with the DB
     * instance, for example
     * <code>arn:aws:iam::123456789012:role/AccessRole</code>.</p>
     */
    inline void SetRoleArn(const Aws::String& value) { m_roleArnHasBeenSet = true; m_roleArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) of the IAM role to associate with the DB
     * instance, for example
     * <code>arn:aws:iam::123456789012:role/AccessRole</code>.</p>
     */
    inline void SetRoleArn(Aws::String&& value) { m_roleArnHasBeenSet = true; m_roleArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the IAM role to associate with the DB
     * instance, for example
     * <code>arn:aws:iam::123456789012:role/AccessRole</code>.</p>
     */
    inline void SetRoleArn(const char* value) { m_roleArnHasBeenSet = true; m_roleArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the IAM role to associate with the DB
     * instance, for example
     * <code>arn:aws:iam::123456789012:role/AccessRole</code>.</p>
     */
    inline AddRoleToDBInstanceRequest& WithRoleArn(const Aws::String& value) { SetRoleArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the IAM role to associate with the DB
     * instance, for example
     * <code>arn:aws:iam::123456789012:role/AccessRole</code>.</p>
     */
    inline AddRoleToDBInstanceRequest& WithRoleArn(Aws::String&& value) { SetRoleArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the IAM role to associate with the DB
     * instance, for example
     * <code>arn:aws:iam::123456789012:role/AccessRole</code>.</p>
     */
    inline AddRoleToDBInstanceRequest& WithRoleArn(const char* value) { SetRoleArn(value); return *this;}


    /**
     * <p>The name of the feature for the DB instance that the IAM role is to be
     * associated with. For information about supported feature names, see
     * <a>DBEngineVersion</a>.</p>
     */
    inline const Aws::String& GetFeatureName() const{ return m_featureName; }

    /**
     * <p>The name of the feature for the DB instance that the IAM role is to be
     * associated with. For information about supported feature names, see
     * <a>DBEngineVersion</a>.</p>
     */
    inline bool FeatureNameHasBeenSet() const { return m_featureNameHasBeenSet; }

    /**
     * <p>The name of the feature for the DB instance that the IAM role is to be
     * associated with. For information about supported feature names, see
     * <a>DBEngineVersion</a>.</p>
     */
    inline void SetFeatureName(const Aws::String& value) { m_featureNameHasBeenSet = true; m_featureName = value; }

    /**
     * <p>The name of the feature for the DB instance that the IAM role is to be
     * associated with. For information about supported feature names, see
     * <a>DBEngineVersion</a>.</p>
     */
    inline void SetFeatureName(Aws::String&& value) { m_featureNameHasBeenSet = true; m_featureName = std::move(value); }

    /**
     * <p>The name of the feature for the DB instance that the IAM role is to be
     * associated with. For information about supported feature names, see
     * <a>DBEngineVersion</a>.</p>
     */
    inline void SetFeatureName(const char* value) { m_featureNameHasBeenSet = true; m_featureName.assign(value); }

    /**
     * <p>The name of the feature for the DB instance that the IAM role is to be
     * associated with. For information about supported feature names, see
     * <a>DBEngineVersion</a>.</p>
     */
    inline AddRoleToDBInstanceRequest& WithFeatureName(const Aws::String& value) { SetFeatureName(value); return *this;}

    /**
     * <p>The name of the feature for the DB instance that the IAM role is to be
     * associated with. For information about supported feature names, see
     * <a>DBEngineVersion</a>.</p>
     */
    inline AddRoleToDBInstanceRequest& WithFeatureName(Aws::String&& value) { SetFeatureName(std::move(value)); return *this;}

    /**
     * <p>The name of the feature for the DB instance that the IAM role is to be
     * associated with. For information about supported feature names, see
     * <a>DBEngineVersion</a>.</p>
     */
    inline AddRoleToDBInstanceRequest& WithFeatureName(const char* value) { SetFeatureName(value); return *this;}

  private:

    Aws::String m_dBInstanceIdentifier;
    bool m_dBInstanceIdentifierHasBeenSet = false;

    Aws::String m_roleArn;
    bool m_roleArnHasBeenSet = false;

    Aws::String m_featureName;
    bool m_featureNameHasBeenSet = false;
  };

} // namespace Model
} // namespace RDS
} // namespace Aws
