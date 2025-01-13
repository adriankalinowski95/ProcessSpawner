import { Injectable } from "@angular/core";
import { AuthService } from "./auth.service";
import { ActivatedRouteSnapshot, Router, RouterStateSnapshot } from "@angular/router";

@Injectable({ 
    providedIn: 'root' 
})
export class RoleGuard {
  constructor(private authService: AuthService, private router: Router) {}

    canActivate(route: ActivatedRouteSnapshot, state: RouterStateSnapshot) {
        const expectedRoles = route.data['roles'] as string[] | undefined;
        if (!expectedRoles || expectedRoles.length === 0) {
            return true;
        }

        const userRoles = this.authService.getUserRoles();
        const hasRole = userRoles?.some(role => expectedRoles.includes(role));
        if (!hasRole) {
            this.router.navigate(['/dashboard']);  
             
            return false;
        }

        return true;
    }
}