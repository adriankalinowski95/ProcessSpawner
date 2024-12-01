import { AuthHTTPService } from "../services/auth-http.service";

export class AuthFactory {
    constructor(private authHttpService: AuthHTTPService) {}

    createGetTokenFacade() {
        
    }
}