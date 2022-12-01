﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/textract/TextractClient.h>
#include <aws/textract/TextractErrorMarshaller.h>
#include <aws/textract/TextractEndpointProvider.h>
#include <aws/textract/model/AnalyzeDocumentRequest.h>
#include <aws/textract/model/AnalyzeExpenseRequest.h>
#include <aws/textract/model/AnalyzeIDRequest.h>
#include <aws/textract/model/DetectDocumentTextRequest.h>
#include <aws/textract/model/GetDocumentAnalysisRequest.h>
#include <aws/textract/model/GetDocumentTextDetectionRequest.h>
#include <aws/textract/model/GetExpenseAnalysisRequest.h>
#include <aws/textract/model/StartDocumentAnalysisRequest.h>
#include <aws/textract/model/StartDocumentTextDetectionRequest.h>
#include <aws/textract/model/StartExpenseAnalysisRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Textract;
using namespace Aws::Textract::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* TextractClient::SERVICE_NAME = "textract";
const char* TextractClient::ALLOCATION_TAG = "TextractClient";

TextractClient::TextractClient(const Textract::TextractClientConfiguration& clientConfiguration,
                               std::shared_ptr<TextractEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<TextractErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

TextractClient::TextractClient(const AWSCredentials& credentials,
                               std::shared_ptr<TextractEndpointProviderBase> endpointProvider,
                               const Textract::TextractClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<TextractErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

TextractClient::TextractClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               std::shared_ptr<TextractEndpointProviderBase> endpointProvider,
                               const Textract::TextractClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<TextractErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  TextractClient::TextractClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<TextractErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<TextractEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

TextractClient::TextractClient(const AWSCredentials& credentials,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<TextractErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<TextractEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

TextractClient::TextractClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<TextractErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<TextractEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
TextractClient::~TextractClient()
{
}

std::shared_ptr<TextractEndpointProviderBase>& TextractClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void TextractClient::init(const Textract::TextractClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Textract");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void TextractClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AnalyzeDocumentOutcome TextractClient::AnalyzeDocument(const AnalyzeDocumentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AnalyzeDocument, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AnalyzeDocument, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AnalyzeDocumentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AnalyzeDocumentOutcomeCallable TextractClient::AnalyzeDocumentCallable(const AnalyzeDocumentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::AnalyzeDocument, this, request, m_executor.get());
}

void TextractClient::AnalyzeDocumentAsync(const AnalyzeDocumentRequest& request, const AnalyzeDocumentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::AnalyzeDocument, this, request, handler, context, m_executor.get());
}

AnalyzeExpenseOutcome TextractClient::AnalyzeExpense(const AnalyzeExpenseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AnalyzeExpense, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AnalyzeExpense, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AnalyzeExpenseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AnalyzeExpenseOutcomeCallable TextractClient::AnalyzeExpenseCallable(const AnalyzeExpenseRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::AnalyzeExpense, this, request, m_executor.get());
}

void TextractClient::AnalyzeExpenseAsync(const AnalyzeExpenseRequest& request, const AnalyzeExpenseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::AnalyzeExpense, this, request, handler, context, m_executor.get());
}

AnalyzeIDOutcome TextractClient::AnalyzeID(const AnalyzeIDRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AnalyzeID, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AnalyzeID, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AnalyzeIDOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AnalyzeIDOutcomeCallable TextractClient::AnalyzeIDCallable(const AnalyzeIDRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::AnalyzeID, this, request, m_executor.get());
}

void TextractClient::AnalyzeIDAsync(const AnalyzeIDRequest& request, const AnalyzeIDResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::AnalyzeID, this, request, handler, context, m_executor.get());
}

DetectDocumentTextOutcome TextractClient::DetectDocumentText(const DetectDocumentTextRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DetectDocumentText, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DetectDocumentText, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DetectDocumentTextOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DetectDocumentTextOutcomeCallable TextractClient::DetectDocumentTextCallable(const DetectDocumentTextRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::DetectDocumentText, this, request, m_executor.get());
}

void TextractClient::DetectDocumentTextAsync(const DetectDocumentTextRequest& request, const DetectDocumentTextResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::DetectDocumentText, this, request, handler, context, m_executor.get());
}

GetDocumentAnalysisOutcome TextractClient::GetDocumentAnalysis(const GetDocumentAnalysisRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDocumentAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDocumentAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetDocumentAnalysisOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetDocumentAnalysisOutcomeCallable TextractClient::GetDocumentAnalysisCallable(const GetDocumentAnalysisRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::GetDocumentAnalysis, this, request, m_executor.get());
}

void TextractClient::GetDocumentAnalysisAsync(const GetDocumentAnalysisRequest& request, const GetDocumentAnalysisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::GetDocumentAnalysis, this, request, handler, context, m_executor.get());
}

GetDocumentTextDetectionOutcome TextractClient::GetDocumentTextDetection(const GetDocumentTextDetectionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDocumentTextDetection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDocumentTextDetection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetDocumentTextDetectionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetDocumentTextDetectionOutcomeCallable TextractClient::GetDocumentTextDetectionCallable(const GetDocumentTextDetectionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::GetDocumentTextDetection, this, request, m_executor.get());
}

void TextractClient::GetDocumentTextDetectionAsync(const GetDocumentTextDetectionRequest& request, const GetDocumentTextDetectionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::GetDocumentTextDetection, this, request, handler, context, m_executor.get());
}

GetExpenseAnalysisOutcome TextractClient::GetExpenseAnalysis(const GetExpenseAnalysisRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExpenseAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExpenseAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetExpenseAnalysisOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetExpenseAnalysisOutcomeCallable TextractClient::GetExpenseAnalysisCallable(const GetExpenseAnalysisRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::GetExpenseAnalysis, this, request, m_executor.get());
}

void TextractClient::GetExpenseAnalysisAsync(const GetExpenseAnalysisRequest& request, const GetExpenseAnalysisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::GetExpenseAnalysis, this, request, handler, context, m_executor.get());
}

StartDocumentAnalysisOutcome TextractClient::StartDocumentAnalysis(const StartDocumentAnalysisRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartDocumentAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartDocumentAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartDocumentAnalysisOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartDocumentAnalysisOutcomeCallable TextractClient::StartDocumentAnalysisCallable(const StartDocumentAnalysisRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::StartDocumentAnalysis, this, request, m_executor.get());
}

void TextractClient::StartDocumentAnalysisAsync(const StartDocumentAnalysisRequest& request, const StartDocumentAnalysisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::StartDocumentAnalysis, this, request, handler, context, m_executor.get());
}

StartDocumentTextDetectionOutcome TextractClient::StartDocumentTextDetection(const StartDocumentTextDetectionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartDocumentTextDetection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartDocumentTextDetection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartDocumentTextDetectionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartDocumentTextDetectionOutcomeCallable TextractClient::StartDocumentTextDetectionCallable(const StartDocumentTextDetectionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::StartDocumentTextDetection, this, request, m_executor.get());
}

void TextractClient::StartDocumentTextDetectionAsync(const StartDocumentTextDetectionRequest& request, const StartDocumentTextDetectionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::StartDocumentTextDetection, this, request, handler, context, m_executor.get());
}

StartExpenseAnalysisOutcome TextractClient::StartExpenseAnalysis(const StartExpenseAnalysisRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartExpenseAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartExpenseAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartExpenseAnalysisOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartExpenseAnalysisOutcomeCallable TextractClient::StartExpenseAnalysisCallable(const StartExpenseAnalysisRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &TextractClient::StartExpenseAnalysis, this, request, m_executor.get());
}

void TextractClient::StartExpenseAnalysisAsync(const StartExpenseAnalysisRequest& request, const StartExpenseAnalysisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&TextractClient::StartExpenseAnalysis, this, request, handler, context, m_executor.get());
}

