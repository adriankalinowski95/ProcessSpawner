export enum BaseResponseStatus {
    Ok,
    Error
}

export interface BaseResponse {
    status: BaseResponseStatus;
    errorMessage: string;
}