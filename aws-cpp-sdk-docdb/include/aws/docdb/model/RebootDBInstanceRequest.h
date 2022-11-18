﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/docdb/DocDB_EXPORTS.h>
#include <aws/docdb/DocDBRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace DocDB
{
namespace Model
{

  /**
   * <p>Represents the input to <a>RebootDBInstance</a>.</p><p><h3>See Also:</h3>  
   * <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/docdb-2014-10-31/RebootDBInstanceMessage">AWS
   * API Reference</a></p>
   */
  class AWS_DOCDB_API RebootDBInstanceRequest : public DocDBRequest
  {
  public:
    RebootDBInstanceRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<RebootDBInstanceRequest> Clone() const
    {
      return Aws::MakeUnique<RebootDBInstanceRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "RebootDBInstance"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The instance identifier. This parameter is stored as a lowercase string.</p>
     * <p>Constraints:</p> <ul> <li> <p>Must match the identifier of an existing
     * <code>DBInstance</code>.</p> </li> </ul>
     */
    inline const Aws::String& GetDBInstanceIdentifier() const{ return m_dBInstanceIdentifier; }

    /**
     * <p>The instance identifier. This parameter is stored as a lowercase string.</p>
     * <p>Constraints:</p> <ul> <li> <p>Must match the identifier of an existing
     * <code>DBInstance</code>.</p> </li> </ul>
     */
    inline bool DBInstanceIdentifierHasBeenSet() const { return m_dBInstanceIdentifierHasBeenSet; }

    /**
     * <p>The instance identifier. This parameter is stored as a lowercase string.</p>
     * <p>Constraints:</p> <ul> <li> <p>Must match the identifier of an existing
     * <code>DBInstance</code>.</p> </li> </ul>
     */
    inline void SetDBInstanceIdentifier(const Aws::String& value) { m_dBInstanceIdentifierHasBeenSet = true; m_dBInstanceIdentifier = value; }

    /**
     * <p>The instance identifier. This parameter is stored as a lowercase string.</p>
     * <p>Constraints:</p> <ul> <li> <p>Must match the identifier of an existing
     * <code>DBInstance</code>.</p> </li> </ul>
     */
    inline void SetDBInstanceIdentifier(Aws::String&& value) { m_dBInstanceIdentifierHasBeenSet = true; m_dBInstanceIdentifier = std::move(value); }

    /**
     * <p>The instance identifier. This parameter is stored as a lowercase string.</p>
     * <p>Constraints:</p> <ul> <li> <p>Must match the identifier of an existing
     * <code>DBInstance</code>.</p> </li> </ul>
     */
    inline void SetDBInstanceIdentifier(const char* value) { m_dBInstanceIdentifierHasBeenSet = true; m_dBInstanceIdentifier.assign(value); }

    /**
     * <p>The instance identifier. This parameter is stored as a lowercase string.</p>
     * <p>Constraints:</p> <ul> <li> <p>Must match the identifier of an existing
     * <code>DBInstance</code>.</p> </li> </ul>
     */
    inline RebootDBInstanceRequest& WithDBInstanceIdentifier(const Aws::String& value) { SetDBInstanceIdentifier(value); return *this;}

    /**
     * <p>The instance identifier. This parameter is stored as a lowercase string.</p>
     * <p>Constraints:</p> <ul> <li> <p>Must match the identifier of an existing
     * <code>DBInstance</code>.</p> </li> </ul>
     */
    inline RebootDBInstanceRequest& WithDBInstanceIdentifier(Aws::String&& value) { SetDBInstanceIdentifier(std::move(value)); return *this;}

    /**
     * <p>The instance identifier. This parameter is stored as a lowercase string.</p>
     * <p>Constraints:</p> <ul> <li> <p>Must match the identifier of an existing
     * <code>DBInstance</code>.</p> </li> </ul>
     */
    inline RebootDBInstanceRequest& WithDBInstanceIdentifier(const char* value) { SetDBInstanceIdentifier(value); return *this;}


    /**
     * <p> When <code>true</code>, the reboot is conducted through a Multi-AZ failover.
     * </p> <p>Constraint: You can't specify <code>true</code> if the instance is not
     * configured for Multi-AZ.</p>
     */
    inline bool GetForceFailover() const{ return m_forceFailover; }

    /**
     * <p> When <code>true</code>, the reboot is conducted through a Multi-AZ failover.
     * </p> <p>Constraint: You can't specify <code>true</code> if the instance is not
     * configured for Multi-AZ.</p>
     */
    inline bool ForceFailoverHasBeenSet() const { return m_forceFailoverHasBeenSet; }

    /**
     * <p> When <code>true</code>, the reboot is conducted through a Multi-AZ failover.
     * </p> <p>Constraint: You can't specify <code>true</code> if the instance is not
     * configured for Multi-AZ.</p>
     */
    inline void SetForceFailover(bool value) { m_forceFailoverHasBeenSet = true; m_forceFailover = value; }

    /**
     * <p> When <code>true</code>, the reboot is conducted through a Multi-AZ failover.
     * </p> <p>Constraint: You can't specify <code>true</code> if the instance is not
     * configured for Multi-AZ.</p>
     */
    inline RebootDBInstanceRequest& WithForceFailover(bool value) { SetForceFailover(value); return *this;}

  private:

    Aws::String m_dBInstanceIdentifier;
    bool m_dBInstanceIdentifierHasBeenSet = false;

    bool m_forceFailover;
    bool m_forceFailoverHasBeenSet = false;
  };

} // namespace Model
} // namespace DocDB
} // namespace Aws
