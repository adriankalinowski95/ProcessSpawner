import { BaseResponse } from "./base-response";

export interface ObjectResponse<T> extends BaseResponse {
    object: T;
}