import { Injectable } from "@angular/core";
import { AuthService } from "./auth.service";
import { ActivatedRouteSnapshot, Router, RouterStateSnapshot } from "@angular/router";

@Injectable({ 
    providedIn: 'root' 
})
export class AuthGuard  {
  constructor(private authService: AuthService, private router: Router) {}

    canActivate(route: ActivatedRouteSnapshot, state: RouterStateSnapshot) {
        const currentUser = this.authService.getAuthFromLocalStorage();
        if (currentUser) {
            // logged in so return true
            return true;
        }

        this.router.navigate(['/login'], { queryParams: { returnUrl: state.url } });    
        // not logged in so redirect to login page with the return url
        this.authService.logout();  
        
        return false;
    }
}