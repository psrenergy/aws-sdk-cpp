﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/lambda/Lambda_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace Lambda
{
namespace Model
{

  /**
   * <p>An <a
   * href="https://docs.aws.amazon.com/lambda/latest/dg/configuration-layers.html">Lambda
   * layer</a>.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/lambda-2015-03-31/Layer">AWS API
   * Reference</a></p>
   */
  class AWS_LAMBDA_API Layer
  {
  public:
    Layer();
    Layer(Aws::Utils::Json::JsonView jsonValue);
    Layer& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The Amazon Resource Name (ARN) of the function layer.</p>
     */
    inline const Aws::String& GetArn() const{ return m_arn; }

    /**
     * <p>The Amazon Resource Name (ARN) of the function layer.</p>
     */
    inline bool ArnHasBeenSet() const { return m_arnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) of the function layer.</p>
     */
    inline void SetArn(const Aws::String& value) { m_arnHasBeenSet = true; m_arn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) of the function layer.</p>
     */
    inline void SetArn(Aws::String&& value) { m_arnHasBeenSet = true; m_arn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the function layer.</p>
     */
    inline void SetArn(const char* value) { m_arnHasBeenSet = true; m_arn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the function layer.</p>
     */
    inline Layer& WithArn(const Aws::String& value) { SetArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the function layer.</p>
     */
    inline Layer& WithArn(Aws::String&& value) { SetArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the function layer.</p>
     */
    inline Layer& WithArn(const char* value) { SetArn(value); return *this;}


    /**
     * <p>The size of the layer archive in bytes.</p>
     */
    inline long long GetCodeSize() const{ return m_codeSize; }

    /**
     * <p>The size of the layer archive in bytes.</p>
     */
    inline bool CodeSizeHasBeenSet() const { return m_codeSizeHasBeenSet; }

    /**
     * <p>The size of the layer archive in bytes.</p>
     */
    inline void SetCodeSize(long long value) { m_codeSizeHasBeenSet = true; m_codeSize = value; }

    /**
     * <p>The size of the layer archive in bytes.</p>
     */
    inline Layer& WithCodeSize(long long value) { SetCodeSize(value); return *this;}


    /**
     * <p>The Amazon Resource Name (ARN) for a signing profile version.</p>
     */
    inline const Aws::String& GetSigningProfileVersionArn() const{ return m_signingProfileVersionArn; }

    /**
     * <p>The Amazon Resource Name (ARN) for a signing profile version.</p>
     */
    inline bool SigningProfileVersionArnHasBeenSet() const { return m_signingProfileVersionArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) for a signing profile version.</p>
     */
    inline void SetSigningProfileVersionArn(const Aws::String& value) { m_signingProfileVersionArnHasBeenSet = true; m_signingProfileVersionArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) for a signing profile version.</p>
     */
    inline void SetSigningProfileVersionArn(Aws::String&& value) { m_signingProfileVersionArnHasBeenSet = true; m_signingProfileVersionArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) for a signing profile version.</p>
     */
    inline void SetSigningProfileVersionArn(const char* value) { m_signingProfileVersionArnHasBeenSet = true; m_signingProfileVersionArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) for a signing profile version.</p>
     */
    inline Layer& WithSigningProfileVersionArn(const Aws::String& value) { SetSigningProfileVersionArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) for a signing profile version.</p>
     */
    inline Layer& WithSigningProfileVersionArn(Aws::String&& value) { SetSigningProfileVersionArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) for a signing profile version.</p>
     */
    inline Layer& WithSigningProfileVersionArn(const char* value) { SetSigningProfileVersionArn(value); return *this;}


    /**
     * <p>The Amazon Resource Name (ARN) of a signing job.</p>
     */
    inline const Aws::String& GetSigningJobArn() const{ return m_signingJobArn; }

    /**
     * <p>The Amazon Resource Name (ARN) of a signing job.</p>
     */
    inline bool SigningJobArnHasBeenSet() const { return m_signingJobArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) of a signing job.</p>
     */
    inline void SetSigningJobArn(const Aws::String& value) { m_signingJobArnHasBeenSet = true; m_signingJobArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) of a signing job.</p>
     */
    inline void SetSigningJobArn(Aws::String&& value) { m_signingJobArnHasBeenSet = true; m_signingJobArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of a signing job.</p>
     */
    inline void SetSigningJobArn(const char* value) { m_signingJobArnHasBeenSet = true; m_signingJobArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of a signing job.</p>
     */
    inline Layer& WithSigningJobArn(const Aws::String& value) { SetSigningJobArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of a signing job.</p>
     */
    inline Layer& WithSigningJobArn(Aws::String&& value) { SetSigningJobArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of a signing job.</p>
     */
    inline Layer& WithSigningJobArn(const char* value) { SetSigningJobArn(value); return *this;}

  private:

    Aws::String m_arn;
    bool m_arnHasBeenSet = false;

    long long m_codeSize;
    bool m_codeSizeHasBeenSet = false;

    Aws::String m_signingProfileVersionArn;
    bool m_signingProfileVersionArnHasBeenSet = false;

    Aws::String m_signingJobArn;
    bool m_signingJobArnHasBeenSet = false;
  };

} // namespace Model
} // namespace Lambda
} // namespace Aws
