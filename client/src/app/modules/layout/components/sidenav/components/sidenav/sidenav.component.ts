import { Component, Input } from '@angular/core';
import { MenuItem } from '../../models/menu-item';
import { AuthService } from '../../../../../auth/services/auth.service';
import { isNotNullOrUndefined } from '../../../../../shared/utils/validators';
import { shared } from '../../../../../shared/shared';

@Component({
  selector: 'app-sidenav',
  templateUrl: './sidenav.component.html',
  styleUrl: './sidenav.component.scss'
})
export class SidenavComponent {
    @Input() isExpanded: boolean = false;

    menuItems: MenuItem[] = [
        {
            label: 'Dashboard',
            icon: 'dashboard',
            route: '/dashboard'
        },
        {
            label: 'Process Spawner',
            icon: 'rocket_launch',
            route: '/process-spawner/user/'
        },
        {
            label: 'Process Manager',
            icon: 'manage_accounts',
            route: '/process-manager'
        }
    ];

    constructor(public authService: AuthService) {
        let processSpawnerMenuItem = this.getProcessSpawnerMenuItem();
        if (shared.isNotNullOrUndefined(processSpawnerMenuItem)) {
            const user = this.authService.getAuthFromLocalStorage()?.user;
            if (!shared.isNotNullOrUndefined(user)) {
                return;
            }
            
            // @Todo repair a active route navbar - its not working for process-spawner/manager/{id}
            processSpawnerMenuItem.route = '/process-spawner/user/' + user.id;
        }
    }

    private getProcessSpawnerMenuItem(): MenuItem | undefined {
        return this.menuItems.find(item => item.route === '/process-spawner/user/');
    }
}
